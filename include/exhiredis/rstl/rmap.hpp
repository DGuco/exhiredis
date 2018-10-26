//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RMAP_H
#define EXHIREDIS_RMAP_H

#include <memory>
#include <utility>
#include "exhiredis/redis_conn.hpp"

namespace exhiredis
{
	/**
	 * redis hashes
	 * @tparam key_type
	 * @tparam value_type
	 */
	template<class key_type, class value_type>
	class RMap
	{
	public:
		/**
		 * 构造函数
		 * @param name
		 * @param conn
		 */
		RMap(string name, std::shared_ptr<CRedisConn> conn);
		/**
		 * put
		 * @param key
		 * @param value
		 * @return 0 success otherwise failed
		 */
		int Put(key_type &key, value_type &value);
		/**
		 * 异步 put
		 * @param key
		 * @param value
		 * @return 0 success otherwise failed
		 */
		std::future<int> PutAsync(key_type &key, value_type &value);
		/**
		 * get
		 * @param key
		 * @return
		 */
		value_type Get(key_type &key);
		/**
		 * 异步 get
		 * @param key
		 * @return
		 */
		std::future<value_type> GetAsync(key_type &key);
	private:
		std::string m_sName;
		std::shared_ptr<CRedisConn> m_pRedisConn;
	};

	template<class key_type, class value_type>
	RMap<key_type, value_type>::RMap(string name, std::shared_ptr<CRedisConn> conn)
		: m_sName(std::move(name)),
		  m_pRedisConn(conn)
	{
	}

	template<class key_type, class value_type>
	int RMap<key_type, value_type>::Put(key_type &key, value_type &value)
	{
		return PutAsync(key, value).get( );
	}

	template<class key_type, class value_type>
	value_type RMap<key_type, value_type>::Get(key_type &key)
	{
		return GetAsync(key).get( );
	}

	template<class key_type, class value_type>
	std::future<int> RMap<key_type, value_type>::PutAsync(key_type &key, value_type &value)
	{
		std::shared_ptr<CCommand<int >> command = m_pRedisConn->CreateCommand<int>( );
		return m_pRedisConn->RedisAsyncCommand(std::move(command),
											   "HSET %s %s %s",
											   m_sName,
											   ((IRobject *) (&key))->ToString( ).c_str( ),
											   ((IRobject *) (&value))->ToString( ));
	}

	template<class key_type, class value_type>
	std::future<value_type> RMap<key_type, value_type>::GetAsync(key_type &key)
	{
		std::shared_ptr<CCommand<value_type >> command = m_pRedisConn->CreateCommand<value_type>( );
		return m_pRedisConn->RedisAsyncCommand(std::move(command),
											   "HGET %s %s",
											   m_sName,
											   ((IRobject *) (&key))->ToString( ).c_str( ));
	}
}
#endif //EXHIREDIS_RMAP_H
