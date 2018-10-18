//
// Created by dguco on 18-10-17.
//

#include "redis_conn.h"

namespace exhiredis
{
	CRedisConn::CRedisConn()
	{

	}

	CRedisConn::~CRedisConn()
	{
		if (m_pRedisConrext != nullptr) {
			redisFree(m_pRedisConrext);
		}
	}
}