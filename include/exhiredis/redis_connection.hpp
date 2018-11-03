//
// Created by dguco on 18-10-17.
// redis connection
//

#ifndef EXHIREDIS_REDIS_CONN_H
#define EXHIREDIS_REDIS_CONN_H

#include <stdlib.h>
#include <iostream>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include <memory>
#include <string>
#include <event.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include "utils/signal.hpp"
#include "utils/log.hpp"
#include "command.hpp"
#include "utils/uuid.hpp"
#include "robject/rint.hpp"

using namespace std;

namespace exhiredis
{
	class CRedisConnection
	{
	public:
		enum class eConnState
		{
			NOT_YET_CONNECTED = 0, // Starting state
			CONNECTED = 1,         // Successfully connected
			DISCONNECTED = 2,      // Successfully disconnected
			CONNECT_ERROR = 3,     // Error connecting
			DISCONNECT_ERROR = 4,  // Disconnected on error
			INIT_ERROR = 5         // Failed to init data structures
		};
	public:
		CRedisConnection();
		virtual ~CRedisConnection();
		//连接redis
		bool Connect(const string &host, int port);
		//连接redis
		bool ConnecToUnix(const string &address);
		//获取连接状态
		eConnState GetConnState();
		//设置连接状态
		void SetConnState(eConnState eConnState);
		//执行redis命令 返回是否成功
		std::future<bool> RedisAsyncIsSucceedCommand(const char *cmd, ...);
		//执行redis命令 返回redis integer 指针 or nullptr
		std::future<std::shared_ptr<long long>> RedisAsyncReturnIntCommand(const char *cmd, ...);
		//执行redis命令 返回return_type 指针 or nullptr
		template<class return_type>
		std::future<std::shared_ptr<return_type>> RedisAsyncCommand(const char *cmd, ...);
	private:
		//构建cmd
		std::shared_ptr<CCommand> CreateCommand(const char *cmd, va_list vaList);
		//执行命令
		std::shared_ptr<CCommand> RedisvAsyncCommand(const char *cmd, va_list ap);
		//生成command id
		unsigned long GenCommandId();
		//remove redis command
		std::shared_ptr<CCommand> RemoveCmd(unsigned long cmdId);
	private:
		//连接成功回调
		static void lcb_OnConnectCallback(const redisAsyncContext *context, int status);
		//断开连接回调
		static void lcb_OnDisconnectCallback(const redisAsyncContext *context, int status);
		//redis command callback
		static void lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata);
	private:
		//初始化hiredis
		bool InitHiredis();
		//初始化libevent
		bool InitLibevent();
		//运行eventloop
		bool RunEventLoop();
	private:
		redisAsyncContext *m_pRedisContext;
		std::shared_ptr<thread> m_pEventLoopThread;
		event_base *m_pEventBase;
		string m_sHost;
		int m_iPort;
		string m_sAddress;
		bool m_bIsRuning;
		eConnState m_eConnState;
		std::mutex m_connectLock;
//		std::condition_variable m_connectWaiter;
		std::mutex m_runingLock;
//		std::condition_variable m_runingWaiter;
		atomic_ulong m_cmdId;
		std::unordered_map<unsigned long, std::shared_ptr<CCommand>> m_mCmdMap;
	};

	CRedisConnection::CRedisConnection()
		: m_pEventLoopThread(nullptr),
		  m_pEventBase(nullptr),
		  m_sHost(""),
		  m_iPort(0),
		  m_bIsRuning(false),
		  m_eConnState(eConnState::NOT_YET_CONNECTED)
	{

	}

	CRedisConnection::~CRedisConnection()
	{
		if (m_pRedisContext != nullptr) {
			redisAsyncFree(m_pRedisContext);
		}
		if (m_pEventBase != nullptr) {
			event_base_loopbreak(m_pEventBase);
			event_base_free(m_pEventBase);
		}
		if (m_pEventLoopThread != nullptr) {
			if (m_pEventLoopThread->joinable( )) {
				m_pEventLoopThread->join( );
			}
		}

	}

	bool CRedisConnection::Connect(const string &host, int port)
	{
		m_sHost = host;
		m_iPort = port;
		if (!InitLibevent( )) {
			SetConnState(eConnState::INIT_ERROR);
			return false;
		}
		m_pRedisContext = redisAsyncConnect(m_sHost.c_str( ), m_iPort);
		if (nullptr == m_pRedisContext || m_pRedisContext->err) {
			HIREDIS_LOG_ERROR("Redis connect error: %s\n", m_pRedisContext->errstr);
			SetConnState(eConnState::INIT_ERROR);
			return false;
		}
		if (!InitHiredis( )) {
			return false;
		}
		m_pEventLoopThread = std::make_shared<thread>([this]
													  { RunEventLoop( ); });
//		{
//			unique_lock<mutex> ul(m_runingLock);
//			m_runingWaiter.wait(ul, [this]
//			{
//				lock_guard<mutex> lg(m_connectLock);
//				return m_bIsRuning || m_eConnState == eConnState::CONNECT_ERROR;
//			});
//		}

		return GetConnState( ) == eConnState::CONNECTED;
	}

	bool CRedisConnection::ConnecToUnix(const string &address)
	{
		m_sAddress = address;
		m_pRedisContext = redisAsyncConnectUnix(m_sAddress.c_str( ));
		if (m_pRedisContext->err) {
			HIREDIS_LOG_ERROR("Redis connect error: %s\n", m_pRedisContext->errstr);
			return false;
		}
		m_pEventBase = event_base_new( );
		if (nullptr == m_pEventBase) {
			HIREDIS_LOG_ERROR("Create event base error\n");
		}
		m_pEventLoopThread = std::make_shared<thread>([this]
													  { event_base_dispatch(m_pEventBase); });

//		{
//			unique_lock<mutex> ul(m_runingLock);
//			m_runingWaiter.wait(ul, [this]
//			{
//				lock_guard<mutex> lg(m_connectLock);
//				return m_bIsRuning || m_eConnState == eConnState::CONNECT_ERROR;
//			});
//		}

		return GetConnState( ) == eConnState::CONNECTED;
	}

	CRedisConnection::eConnState CRedisConnection::GetConnState()
	{
		std::lock_guard<mutex> lk(m_connectLock);
		return m_eConnState;
	}

	void CRedisConnection::SetConnState(CRedisConnection::eConnState eConnState)
	{
		{
			std::lock_guard<mutex> lk(m_connectLock);
			m_eConnState = eConnState;
		}
//		m_connectWaiter.notify_all( );
	}

	std::shared_ptr<CCommand> CRedisConnection::RedisvAsyncCommand(const char *cmd,
																   va_list ap)
	{
		std::shared_ptr<CCommand> tmpCommand = CreateCommand(cmd, ap);
		unsigned long cmdId = tmpCommand->GetCommandId( );
		int status = redisvAsyncCommand(m_pRedisContext,
										&CRedisConnection::lcb_OnCommandCallback,
										(void *) cmdId,
										cmd,
										ap);
		if (status == REDIS_OK) {
			m_mCmdMap.insert(std::make_pair(cmdId, tmpCommand));
		}
		else {
			HIREDIS_LOG_ERROR("Push redis command failed,cmd = %s,redis status = %d", cmd, status);
		}
		return tmpCommand;
	}

	std::future<bool> CRedisConnection::RedisAsyncIsSucceedCommand(const char *cmd, ...)
	{
		va_list ap;
		va_start(ap, cmd);
		std::shared_ptr<CCommand> command = RedisvAsyncCommand(cmd, ap);
		va_end(ap);
		return std::async([command]() -> bool
						  {
							  redisReply *res = command->GetPromise( )->get_future( ).get( );
							  if (res == nullptr) {
								  return false;
							  }
							  if (res->type == REDIS_REPLY_ERROR) {
								  HIREDIS_LOG_ERROR("Redis reply error,error msg: %s", res->str);
								  return false;
							  }

							  return res->type == REDIS_REPLY_STATUS && strcmp(res->str, "OK") == 0;
						  });
	}

	std::future<std::shared_ptr<long long>> CRedisConnection::RedisAsyncReturnIntCommand(const char *cmd, ...)
	{
		va_list ap;
		va_start(ap, cmd);
		std::shared_ptr<CCommand> command = RedisvAsyncCommand(cmd, ap);
		va_end(ap);
		return std::async([command]() -> std::shared_ptr<long long>
						  {
							  redisReply *res = command->GetPromise( )->get_future( ).get( );
							  if (res == nullptr) {
								  return nullptr;
							  }
							  if (res->type == REDIS_REPLY_ERROR) {
								  HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
								  return nullptr;
							  }

							  if (res->type == REDIS_REPLY_NIL) {
								  return nullptr;
							  }

							  if (res->type != REDIS_REPLY_INTEGER) {
								  HIREDIS_LOG_ERROR("Redis reply type is not REDIS_REPLY_INTEGER.");
								  return nullptr;
							  }
							  return std::make_shared<long long>(res->integer);
						  });
	}

	template<class return_type>
	std::future<std::shared_ptr<return_type>> CRedisConnection::RedisAsyncCommand(const char *cmd, ...)
	{
		va_list ap;
		va_start(ap, cmd);
		std::shared_ptr<CCommand> tmpCommand = RedisvAsyncCommand(cmd, ap);
		va_end(ap);
		return std::async([tmpCommand]() -> std::shared_ptr<return_type>
						  {
							  redisReply *res = tmpCommand->GetPromise( )->get_future( ).get( );
							  if (res == nullptr) {
								  return nullptr;
							  }

							  if (res->type == REDIS_REPLY_ERROR) {
								  HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
								  return nullptr;
							  }

							  if (res->type == REDIS_REPLY_NIL) {
								  return nullptr;
							  }

							  if (res->type != REDIS_REPLY_STRING) {
								  HIREDIS_LOG_ERROR("Redis reply type is not REDIS_REPLY_STRING.");
								  return nullptr;
							  }
							  std::shared_ptr<return_type> value = std::make_shared<return_type>( );
							  ((IRobject *) value.get( ))->FromString(res->str, res->len);
							  return value;
						  });
	}

	std::shared_ptr<CCommand> CRedisConnection::RemoveCmd(unsigned long cmdId)
	{
		std::lock_guard<mutex> lock(m_runingLock);
		auto it = m_mCmdMap.find(cmdId);
		std::shared_ptr<CCommand> cmd = nullptr;
		if (it != m_mCmdMap.end( )) {
			cmd = it->second;
			m_mCmdMap.erase(cmdId);
		}
		return cmd;
	}

	void CRedisConnection::lcb_OnConnectCallback(const redisAsyncContext *context, int status)
	{
		CRedisConnection *conn = (CRedisConnection *) context->data;
		if (status != REDIS_OK) {
			HIREDIS_LOG_ERROR("Could not connect to redis,error msg: %s,status: %d", context->errstr, status);
			conn->SetConnState(eConnState::CONNECT_ERROR);
		}
		else {
			HIREDIS_LOG_INFO("Connected to Redis succeed.");
			//禁止hiredis 自动释放reply
			context->c.reader->fn->freeObject = [](void *reply)
			{};
			conn->SetConnState(eConnState::CONNECTED);
		}
	}

	void CRedisConnection::lcb_OnDisconnectCallback(const redisAsyncContext *context, int status)
	{
		CRedisConnection *conn = (CRedisConnection *) context->data;
		if (status != REDIS_OK) {
			HIREDIS_LOG_ERROR("Disconnected from Redis on error: %s ", context->errstr);
			conn->SetConnState(eConnState::DISCONNECT_ERROR);
		}
		else {
			HIREDIS_LOG_ERROR("Disconnected from Redis ok.");
			conn->SetConnState(eConnState::DISCONNECTED);
		}
	}

	void CRedisConnection::lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata)
	{
		CRedisConnection *conn = (CRedisConnection *) context->data;
		unsigned long cmdId = (unsigned long) privdata;
		std::shared_ptr<CCommand> cmd = conn->RemoveCmd(cmdId);
		if (cmd != nullptr) {
			if (reply != nullptr) {
				redisReply *cmdReply = (redisReply *) reply;
				cmd->SetPromiseValue(cmdReply);
			}
			else {
				cmd->SetPromiseValue(nullptr);
			}
		}
	}

	bool CRedisConnection::InitHiredis()
	{
		m_pRedisContext->data = this;
		if (redisLibeventAttach(m_pRedisContext, m_pEventBase) != REDIS_OK) {
			SetConnState(eConnState::INIT_ERROR);
			HIREDIS_LOG_ERROR("redisLibeventAttach failed\n");
			return false;
		}
		if (redisAsyncSetConnectCallback(m_pRedisContext, &CRedisConnection::lcb_OnConnectCallback) != REDIS_OK) {
			SetConnState(eConnState::INIT_ERROR);
			HIREDIS_LOG_ERROR("redisAsyncSetConnectCallback failed\n");
			return false;
		}
		if (redisAsyncSetDisconnectCallback(m_pRedisContext, &CRedisConnection::lcb_OnDisconnectCallback) != REDIS_OK) {
			SetConnState(eConnState::INIT_ERROR);
			HIREDIS_LOG_ERROR("redisAsyncSetDisconnectCallback failed\n");
			return false;
		}
		return true;
	}

	bool CRedisConnection::InitLibevent()
	{
		m_pEventBase = event_base_new( );
		if (nullptr == m_pEventBase) {
			HIREDIS_LOG_ERROR("Create event base error\n");
			return false;
		}
		return true;
	}

	bool CRedisConnection::RunEventLoop()
	{
		ignore_pipe( );
		event_base_dispatch(m_pEventBase);
		return true;
	}

	unsigned long CRedisConnection::GenCommandId()
	{
		return ++m_cmdId;
	}

//构建cmd
	std::shared_ptr<CCommand> CRedisConnection::CreateCommand(const char *cmd, va_list vaList)
	{
		return std::make_shared<CCommand>(GenCommandId( ), cmd, vaList);
	}

}
#endif //EXHIREDIS_REDIS_CONN_H
