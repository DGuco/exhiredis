//
// Created by dguco on 18-10-17.
//

#include "redis_client.hpp"

namespace exhiredis
{
	CRedisClient::CRedisClient()
	{

	}

	CRedisClient::~CRedisClient()
	{
		if (m_pRedisConrext != nullptr) {
			redisFree(m_pRedisConrext);
		}
	}
}
