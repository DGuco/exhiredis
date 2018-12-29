//
// Created by dguco on 18-10-23.
// redis hashes
//

#ifndef EXHIREDIS_RMAP_H
#define EXHIREDIS_RMAP_H

#include <memory>
#include <utility>
#include <future>
#include <exhiredis/command_param.hpp>
#include "exhiredis/redis_connection.hpp"
#include "exhiredis/redis_commands.hpp"
#include "../connection_manager.hpp"

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
    RMap(const string &name, shared_ptr<CRedisConnection> &conn);
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
    future<bool> PutAsync(const key_type &key, const value_type &value);
    /**
     * get
     * @param key
     * @return
     */
    shared_ptr<value_type> Get(const key_type &key);
    /**
     * async get
     * @param key
     * @return
     */
    future<shared_ptr<value_type>> GetAsync(const key_type &key);
private:
    string m_sName;
    shared_ptr<CRedisConnection> m_pRedisConn;
};

template<class key_type, class value_type>
RMap<key_type, value_type>::RMap(const string &name, shared_ptr<CRedisConnection> &conn)
    : m_sName(name),
      m_pRedisConn(conn)
{
}

template<class key_type, class value_type>
bool RMap<key_type, value_type>::Put(const key_type &key, const value_type &value)
{
    return PutAsync(key, value).get();
}

template<class key_type, class value_type>
shared_ptr<value_type> RMap<key_type, value_type>::Get(const key_type &key)
{
    return GetAsync(key).get();
}

template<class key_type, class value_type>
future<bool> RMap<key_type, value_type>::PutAsync(const key_type &key, const value_type &value)
{
    const string &key_str = ((IRobject *) (&key))->ToString();
    const string &value_str = ((IRobject *) (&value))->ToString();
    vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(m_sName),
                                          make_shared<CCmdParam>(key_str),
                                          make_shared<CCmdParam>(key_str.length()),
                                          make_shared<CCmdParam>(value_str),
                                          make_shared<CCmdParam>(value_str.length())};
    return m_pRedisConn->RedisAsyncIsSucceedCommand(redis_commands::HSET, list);
}

template<class key_type, class value_type>
future<shared_ptr<value_type>> RMap<key_type, value_type>::GetAsync(const key_type &key)
{
    const string &key_str = ((IRobject *) (&key))->ToString();
    vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(m_sName),
                                          make_shared<CCmdParam>(key_str),
                                          make_shared<CCmdParam>(key_str.length())};
    return m_pRedisConn->RedisAsyncCommand<value_type>(redis_commands::HGET, list);
}
}
#endif //EXHIREDIS_RMAP_H
