//
// Created by dguco on 18-10-17.
//

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
	}

	void CRedisConn::Connect()
	{
		m_pRedisContext = redisAsyncConnect(m_sAddress.c_str( ), m_iPort);
		if (m_pRedisContext->err) {
			printf("Error: %s\n", c->errstr);
			return 1;
		}
	}
}