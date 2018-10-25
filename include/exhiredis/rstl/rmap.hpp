//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RMAP_H
#define EXHIREDIS_RMAP_H

#include <memory>
#include "exhiredis/redis_conn.hpp"

namespace exhiredis
{
	template<class key_type, class value_type>
	class RMap
	{
	public:
		RMap();
		void Put(key_type &key, value_type &value);
		value_type Get(key_type &key);
	private:
		std::shared_ptr<CRedisConn> m_pRedisConn;
	};

	template<class key_type, class value_type>
	RMap<key_type, value_type>::RMap()
		: m_pRedisConn( )
	{

	}

	template<class key_type, class value_type>
	void RMap<key_type, value_type>::Put(key_type &key, value_type &value)
	{
		printf("ee1231231231 123123123\n");
		return;
//		m_pRedisConn->RedisAsyncCommand("SET %s %s", key.ToString( ).c_str( ), value.ToString( ).c_str( ));
	}

	template<class key_type, class value_type>
	value_type RMap<key_type, value_type>::Get(key_type &key)
	{
		return nullptr;
	}
}
#endif //EXHIREDIS_RMAP_H
