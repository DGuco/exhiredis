//
// Created by dguco on 18-10-23.
// redis hashes
//

#ifndef EXHIREDIS_RMAP_H
#define EXHIREDIS_RMAP_H

#include <memory>
#include <utility>
#include <future>
#include "exhiredis/redis_connection.hpp"
#include "exhiredis/redis_commands.hpp"

namespace exhiredis
{
#define KEY_SIZE 1024
#define VALUE_SIZE 2048

/**
 * redis hashes  key_type value_type (必须实现IRobject接口)
 * @tparam key_type key_typ e must be the subclass of the IRobject ,ex:RBool,RInt,RFloat......
 * @tparam key_type value_type must be the subclass of the IRobject,ex:RBool,RInt,RFloat......
 */
template<class key_type, class value_type>
class RMap
{
public:
    /**
     * RMap
     * @param name
     * @param conn
     */
    RMap(string name, std::shared_ptr<CRedisConnection> conn);
    /**
     * put
     * @param key
     * @param value
     * @return true success false failed
     */
    bool Put(const key_type &key, const value_type &value);
    /**
     * async put
     * @param key
     * @param value
     * @return true success false failed
     */
    std::future<bool> PutAsync(const key_type &key, const value_type &value);
    /**
     * get
     * @param key
     * @return
     */
    std::shared_ptr<value_type> Get(const key_type &key);
    /**
     * async get
     * @param key
     * @return
     */
    std::future<std::shared_ptr<value_type>> GetAsync(const key_type &key);
private:
    std::string m_sName;
    std::shared_ptr<CRedisConnection> m_pRedisConn;
};

template<class key_type, class value_type>
RMap<key_type, value_type>::RMap(string name, std::shared_ptr<CRedisConnection> conn)
    : m_sName(std::move(name)),
      m_pRedisConn(conn)
{
}

template<class key_type, class value_type>
bool RMap<key_type, value_type>::Put(const key_type &key, const value_type &value)
{
    return PutAsync(key, value).get();
}

template<class key_type, class value_type>
std::shared_ptr<value_type> RMap<key_type, value_type>::Get(const key_type &key)
{
    return GetAsync(key).get();
}

template<class key_type, class value_type>
std::future<bool> RMap<key_type, value_type>::PutAsync(const key_type &key, const value_type &value)
{
    char acKey[KEY_SIZE];
    char acValue[VALUE_SIZE];
    int keyLen = ((IRobject *) (&key))->ToString(acKey);
    int valueLen = ((IRobject *) (&value))->ToString(acValue);
    return m_pRedisConn->RedisAsyncIsSucceedCommand(redis_commands::HSET,
                                                    m_sName.c_str(),
                                                    acKey,
                                                    keyLen,
                                                    acValue,
                                                    valueLen);
}

template<class key_type, class value_type>
std::future<std::shared_ptr<value_type>> RMap<key_type, value_type>::GetAsync(const key_type &key)
{
    char acKey[KEY_SIZE];
    int keyLen = ((IRobject *) (&key))->ToString(acKey);
    return m_pRedisConn->RedisAsyncCommand<value_type>(redis_commands::HGET,
                                                       m_sName.c_str(),
                                                       acKey,
                                                       keyLen);
}
}
#endif //EXHIREDIS_RMAP_H
