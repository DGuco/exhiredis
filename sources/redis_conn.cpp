//
// Created by dguco on 18-10-17.
//

#include <hiredis/adapters/libevent.h>
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
		m_pRedisContext = redisAsyncConnect(m_sHost.c_str( ), m_iPort);
		if (nullptr == m_pRedisContext || m_pRedisContext->err) {
			HIREDIS_LOG_ERROR("Redis connect error: %s\n", m_pRedisContext->errstr);
			SetConnState(eConnState::INIT_ERROR);
			return false;
		}
		if (!InitHiredis( )) {
			return false;
		}
		m_pEventBase = event_base_new( );
		if (nullptr == m_pEventBase) {
			HIREDIS_LOG_ERROR("Create event base error\n");
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
		std::lock_guard<mutex> lk(m_connectLock);
		m_eConnState = eConnState;
	}

	void CRedisConn::lcb_OnConnectCallback(const redisAsyncContext *c, int status)
	{

	}

	void CRedisConn::lcb_OnDisconnectCallback(const redisAsyncContext *c, int status)
	{

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

	}
	bool CRedisConn::RunEventLoop()
	{
		event_base_dispatch(m_pEventBase);
	}

}