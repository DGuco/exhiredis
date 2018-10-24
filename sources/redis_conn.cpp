//
// Created by dguco on 18-10-17.
//

#include <hiredis/adapters/libevent.h>
#include <utils/signal.h>
#include "redis_conn.h"
#include "utils/log.h"

namespace exhiredis
{
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
		{
			unique_lock<mutex> ul(m_runingLock);
			m_runingWaiter.wait(ul, [this]
			{
				lock_guard<mutex> lg(m_connectLock);
				return m_bIsRuning || m_eConnState == eConnState::CONNECT_ERROR;
			});
		}

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

		{
			unique_lock<mutex> ul(m_runingLock);
			m_runingWaiter.wait(ul, [this]
			{
				lock_guard<mutex> lg(m_connectLock);
				return m_bIsRuning || m_eConnState == eConnState::CONNECT_ERROR;
			});
		}

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
		m_connectWaiter.notify_all( );
	}

	std::shared_ptr<CCommand> &CRedisConn::RedisAsyncCommand(char *cmd, ...)
	{
		va_list ap;
		va_start(ap, cmd);
		unsigned long cmdId = CCommand::GenCommandId( );
		int status = redisvAsyncCommand(m_pRedisContext,
										&CRedisConn::lcb_OnCommandCallback,
										(void *) cmdId,
										cmd,
										ap);
		std::shared_ptr<CCommand> tmpCmd = std::make_shared<CCommand>(cmd, cmdId);
		va_end(ap);
		if (status == REDIS_OK) {
			m_mCmdMap.insert(std::make_pair(tmpCmd->GetCommandId( ), std::move(tmpCmd)));
		}
		else {
			HIREDIS_LOG_ERROR("Push redis command failed,cmd = %s,redis status = %d", cmd, status);
		}
		return tmpCmd;
	}

	std::shared_ptr<CCommand> CRedisConn::FindCmd(unsigned long cmdId)
	{
		std::lock_guard<mutex> lock(m_runingLock);
		auto it = m_mCmdMap.find(cmdId);
		if (it != m_mCmdMap.end( )) {
			return it->second;
		}
		return nullptr;
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
				cmd->GetPromise( ).set_value(cmd->GetObj( ).FromString(cmdReply->str));
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
	}

	bool CRedisConn::InitLibevent()
	{
		m_pEventBase = event_base_new( );
		if (nullptr == m_pEventBase) {
			HIREDIS_LOG_ERROR("Create event base error\n");
		}
	}

	bool CRedisConn::RunEventLoop()
	{
		ignore_pipe( );
		event_base_dispatch(m_pEventBase);
	}

}