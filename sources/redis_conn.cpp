//
// Created by dguco on 18-10-17.
//

#include <hiredis/adapters/libevent.h>
#include "redis_conn.h"

namespace exhiredis
{
	CRedisConn::CRedisConn()
	{
		m_sAddress = "127.0.0.1";
		m_iPort = 6379;
	}

	CRedisConn::CRedisConn(const string &sAddress, int iPort)
		: m_sAddress(sAddress), m_iPort(iPort)
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
	}

	bool CRedisConn::Connect()
	{
		m_pRedisContext = redisAsyncConnect(m_sAddress.c_str( ), m_iPort);
		if (m_pRedisContext->err) {
			printf("Redis connect error: %s\n", m_pRedisContext->errstr);
			return false;
		}
		m_pEventBase = event_base_new( );
		if (nullptr == m_pEventBase) {
			printf("Create event base error\n");
		}
		redisLibeventAttach(m_pRedisContext, m_pEventBase);
		redisAsyncSetConnectCallback(m_pRedisContext, &CRedisConn::lcb_OnConnectCallback);
		redisAsyncSetDisconnectCallback(m_pRedisContext, &CRedisConn::lcb_OnDisconnectCallback);
		event_base_dispatch(m_pEventBase);
		return true;
	}

	void CRedisConn::lcb_OnConnectCallback(const redisAsyncContext *c, int status)
	{

	}

	void CRedisConn::lcb_OnDisconnectCallback(const redisAsyncContext *c, int status)
	{

	}
}