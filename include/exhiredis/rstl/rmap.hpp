//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RMAP_H
#define EXHIREDIS_RMAP_H

#include <memory>
#include <utility>
#include <future>
#include "exhiredis/redis_conn.hpp"

namespace exhiredis
{
#define KEY_SIZE 1024
#define VALUE_SIZE 2048

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
		int Put(const key_type &key, const value_type &value);
		/**
		 * 异步 put
		 * @param key
		 * @param value
		 * @return 0 success otherwise failed
		 */
		std::future<int> PutAsync(const key_type &key, const value_type &value);
		/**
		 * get
		 * @param key
		 * @return
		 */
		std::shared_ptr<value_type> Get(const key_type &key);
		/**
		 * 异步 get
		 * @param key
		 * @return
		 */
		std::future<std::shared_ptr<value_type>> GetAsync(const key_type &key);
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
	int RMap<key_type, value_type>::Put(const key_type &key, const value_type &value)
	{
		return PutAsync(key, value).get( );
	}

	template<class key_type, class value_type>
	std::shared_ptr<value_type> RMap<key_type, value_type>::Get(const key_type &key)
	{
		return GetAsync(key).get( );
	}

	template<class key_type, class value_type>
	std::future<int> RMap<key_type, value_type>::PutAsync(const key_type &key, const value_type &value)
	{
		char acKey[KEY_SIZE];
		char acValue[VALUE_SIZE];
		int keyLen = ((IRobject *) (&key))->ToString(acKey);
		int valueLen = ((IRobject *) (&value))->ToString(acValue);
		std::shared_ptr<CCommand> command = m_pRedisConn->RedisAsyncCommand("HSET %s %b %b",
																			m_sName.c_str( ),
																			acKey,
																			keyLen,
																			acValue,
																			valueLen);
		return std::async([command]() -> int
						  {
							  redisReply *res = command->GetPromise( )->get_future( ).get( );
							  if (res == nullptr) {
								  return -1;
							  }
							  return static_cast<int>(res->integer);
						  });
	}

	template<class key_type, class value_type>
	std::future<std::shared_ptr<value_type>> RMap<key_type, value_type>::GetAsync(const key_type &key)
	{
		char acKey[KEY_SIZE];
		int keyLen = ((IRobject *) (&key))->ToString(acKey);
		std::shared_ptr<CCommand> tmpCommand = m_pRedisConn->RedisAsyncCommand("HGET %s %b",
																			   m_sName.c_str( ),
																			   acKey,
																			   keyLen);
		return std::async([tmpCommand]() -> std::shared_ptr<value_type>
						  {
							  redisReply *res = tmpCommand->GetPromise( )->get_future( ).get( );
							  if (res == nullptr) {
								  return nullptr;
							  }
							  std::shared_ptr<value_type> value = std::make_shared<value_type>( );
							  ((IRobject *) value.get( ))->FromString(res->str, res->len);
							  printf("==========GetAsync=========\n");
							  return value;
						  });
	}
}
#endif //EXHIREDIS_RMAP_H
