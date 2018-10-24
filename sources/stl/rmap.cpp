//
// Created by dguco on 18-10-23.
//

#include <utility>

#include "rstl/rmap.h"

namespace exhiredis
{
	RMap::RMap(std::shared_ptr<CRedisConn> conn)
		: m_pRedisConn(std::move(conn))
	{

	}

	void RMap::Put(IRobject &key, IRobject &value)
	{
		m_pRedisConn->RedisAsyncCommand("SET %s %s", key.ToString( ).c_str( ), value.ToString( ).c_str( ));
	}

	void RMap::Get(IRobject &key)
	{

	}
}