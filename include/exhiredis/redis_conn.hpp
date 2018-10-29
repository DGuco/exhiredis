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
#include "exhiredis/utils/signal.hpp"
#include "utils/log.hpp"
#include "command.hpp"
#include "utils/uuid.hpp"
#include "robject/rint.hpp"

using namespace std;

namespace exhiredis
{
	class CRedisConn
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
		CRedisConn();
		virtual ~CRedisConn();
		//连接redis
		bool Connect(const string &host, int port);
		//连接redis
		bool ConnecToUnix(const string &address);
		//获取连接状态
		eConnState GetConnState();
		//设置连接状态
		void SetConnState(eConnState eConnState);
		//执行命令
		std::shared_ptr<CCommand> RedisAsyncCommand(char *cmd, ...);
		//构建cmd
		std::shared_ptr<CCommand> CreateCommand();
	private:
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

	CRedisConn::CRedisConn()
		: m_pEventLoopThread(nullptr),
		  m_pEventBase(nullptr),
		  m_sHost(""),
		  m_iPort(0),
		  m_bIsRuning(false),
		  m_eConnState(eConnState::NOT_YET_CONNECTED)
	{

	}

	CRedisConn::~CRedisConn()
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

	bool CRedisConn::Connect(const string &host, int port)
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

	bool CRedisConn::ConnecToUnix(const string &address)
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

	CRedisConn::eConnState CRedisConn::GetConnState()
	{
		std::lock_guard<mutex> lk(m_connectLock);
		return m_eConnState;
	}

	void CRedisConn::SetConnState(CRedisConn::eConnState eConnState)
	{
		{
			std::lock_guard<mutex> lk(m_connectLock);
			m_eConnState = eConnState;
		}
//		m_connectWaiter.notify_all( );
	}

	std::shared_ptr<CCommand> CRedisConn::RedisAsyncCommand(char *cmd,
															...)
	{
		va_list ap;
		va_start(ap, cmd);
		std::shared_ptr<CCommand> tmpCommand = CreateCommand( );
		unsigned long cmdId = tmpCommand->GetCommandId( );
		int status = redisvAsyncCommand(m_pRedisContext,
										&CRedisConn::lcb_OnCommandCallback,
										(void *) cmdId,
										cmd,
										ap);
		va_end(ap);
		if (status == REDIS_OK) {
			m_mCmdMap.insert(std::make_pair(cmdId, tmpCommand));
		}
		else {
			HIREDIS_LOG_ERROR("Push redis command failed,cmd = %s,redis status = %d", cmd, status);
		}
		return tmpCommand;
	}

	std::shared_ptr<CCommand> CRedisConn::RemoveCmd(unsigned long cmdId)
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

	void CRedisConn::lcb_OnConnectCallback(const redisAsyncContext *context, int status)
	{
		CRedisConn *conn = (CRedisConn *) context->data;
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

	void CRedisConn::lcb_OnDisconnectCallback(const redisAsyncContext *context, int status)
	{
		CRedisConn *conn = (CRedisConn *) context->data;
		if (status != REDIS_OK) {
			HIREDIS_LOG_ERROR("Disconnected from Redis on error: %s ", context->errstr);
			conn->SetConnState(eConnState::DISCONNECT_ERROR);
		}
		else {
			HIREDIS_LOG_ERROR("Disconnected from Redis ok.");
			conn->SetConnState(eConnState::DISCONNECTED);
		}
	}

	void CRedisConn::lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata)
	{
		CRedisConn *conn = (CRedisConn *) context->data;
		unsigned long cmdId = (unsigned long) privdata;
		std::shared_ptr<CCommand> cmd = conn->RemoveCmd(cmdId);
		if (cmd != nullptr) {
			if (reply != nullptr) {
				redisReply *cmdReply = (redisReply *) reply;
				printf("===============cmdReply = %s\n", cmdReply->str);
				cmd->GetPromise( )->set_value(cmdReply);
			}
		}
	}

	bool CRedisConn::InitHiredis()
	{
		m_pRedisContext->data = this;
		if (redisLibeventAttach(m_pRedisContext, m_pEventBase) != REDIS_OK) {
			SetConnState(eConnState::INIT_ERROR);
			HIREDIS_LOG_ERROR("redisLibeventAttach failed\n");
			return false;
		}
		if (redisAsyncSetConnectCallback(m_pRedisContext, &CRedisConn::lcb_OnConnectCallback) != REDIS_OK) {
			SetConnState(eConnState::INIT_ERROR);
			HIREDIS_LOG_ERROR("redisAsyncSetConnectCallback failed\n");
			return false;
		}
		if (redisAsyncSetDisconnectCallback(m_pRedisContext, &CRedisConn::lcb_OnDisconnectCallback) != REDIS_OK) {
			SetConnState(eConnState::INIT_ERROR);
			HIREDIS_LOG_ERROR("redisAsyncSetDisconnectCallback failed\n");
			return false;
		}
		return true;
	}

	bool CRedisConn::InitLibevent()
	{
		m_pEventBase = event_base_new( );
		if (nullptr == m_pEventBase) {
			HIREDIS_LOG_ERROR("Create event base error\n");
			return false;
		}
		return true;
	}

	bool CRedisConn::RunEventLoop()
	{
		ignore_pipe( );
		event_base_dispatch(m_pEventBase);
		return true;
	}

	unsigned long CRedisConn::GenCommandId()
	{
		return ++m_cmdId;
	}

	//构建cmd
	std::shared_ptr<CCommand> CRedisConn::CreateCommand()
	{
		return std::make_shared<CCommand>(GenCommandId( ));
	}

}
#endif //EXHIREDIS_REDIS_CONN_H
