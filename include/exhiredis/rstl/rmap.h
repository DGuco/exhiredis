//
// Created by dguco on 18-10-23.
// redis hashes
//

#ifndef EXHIREDIS_RMAP_H
#define EXHIREDIS_RMAP_H

#include <memory>
#include <utility>
#include <future>
#include <list>
#include "exhiredis/comman_def.h"
#include "param.h"
#include "exhiredis/redis_commands.h"
#include "exhiredis/connection_manager.h"

namespace exhiredis {
/**
 * redis hashes  key_type value_type (必须实现IRobject接口)
 * @tparam key_type key_typ e must be the subclass of the IRobject ,ex:RBool,RInt,RFloat......
 * @tparam key_type value_type must be the subclass of the IRobject,ex:RBool,RInt,RFloat......
 */

    template<class key_type, class value_type>
    class RMap {
    public:
        /**
         * RMap
         * @param name
         * @param conn
         */
        RMap(const string &name, shared_ptr<CConnectionManager> &conn);

        /**
         * HSET
         * @param key
         * @param value
         * @return true success false failed
         */
        bool Put(const key_type &key, const value_type &value);

        /**
         * async HSET
         * @param key
         * @param value
         * @return true success false failed
         */
        future<bool> PutAsync(const key_type &key, const value_type &value);

        /**
         * HGET
         * @param key
         * @return
         */
        shared_ptr<value_type> Get(const key_type &key);

        /**
         * async HGET
         * @param key
         * @return
         */
        future<shared_ptr<value_type>> GetAsync(const key_type &key);

        /**
        * HKEYS
        * @return all map keys
        */
        shared_ptr<list<key_type>> Keys();

        /**
         * async HKEYS
         * @return
         */
        future<shared_ptr<list<key_type>>> KeysAsync();

        /**
         * HGETALL
         * @return shared_ptr<list<pair<key_type, value_type>>>
         */
        shared_ptr<list<pair<key_type, value_type>>> GetAll();

        /**
         * async HGETALL
         * @return shared_ptr<list<pair<key_type, value_type>>>
         */
        future<shared_ptr<list<pair<key_type, value_type>>>> GetAllAsync();

        /**
         * HEXISTS
         * @param key
         * @return true or false
         */
        shared_ptr<bool> Exists(const key_type &key);

        /**
         * async HEXISTS
         * @param key
         * @return true or false
         */
        future<shared_ptr<bool>> ExistsAsync(const key_type &key);

    private:
        string m_sName;
        shared_ptr<CConnectionManager> m_pConnectionManager;
    };

    template<class key_type, class value_type>
    RMap<key_type, value_type>::RMap(const string &name, shared_ptr<CConnectionManager> &conn)
            : m_sName(name),
              m_pConnectionManager(conn)
    {}

    template<class key_type, class value_type>
    bool RMap<key_type, value_type>::Put(const key_type &key, const value_type &value)
    {
        const string &key_str = CParam<key_type>(key).ToString();
        const string &value_str = CParam<value_type>(value).ToString();
        return m_pConnectionManager->ExecuteCommand(
                [key_str,value_str](shared_ptr<CRedisConnection> conn) -> bool {
                    shared_ptr<CRedisReply> reply = conn->SendCommand({(CRedisCommands::HSET, key_str, value_str)});
                    return reply->IntegerValue() == 1;
                });
    }

    template<class key_type, class value_type>
    future<bool> RMap<key_type, value_type>::PutAsync(const key_type &key, const value_type &value) {
        const string key_str = CParam<key_type>(key).ToString();
        const string value_str = CParam<value_type>(value).ToString();
        return m_pConnectionManager->AsyncExecuteCommand(
                [key_str,value_str](shared_ptr<CRedisConnection> conn) -> bool {
                    shared_ptr<CRedisReply> reply = conn->SendCommand({CRedisCommands::HSET, key_str, value_str});
                    return reply->IntegerValue() == 1;
                });
    }

    template<class key_type, class value_type>
    shared_ptr<value_type> RMap<key_type, value_type>::Get(const key_type &key) {
        const string key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->ExecuteCommand(
                [key_str](shared_ptr<CRedisConnection> conn) -> value_type {
                    shared_ptr<CRedisReply> reply = conn->SendCommand({CRedisCommands::HGET, key_str});
                    return CParam<value_type>(reply->StrValue()).value;
                });
    }

    template<class key_type, class value_type>
    future<shared_ptr<value_type>> RMap<key_type, value_type>::GetAsync(const key_type &key) {
        const string key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->AsyncExecuteCommand(
                [key_str](shared_ptr<CRedisConnection> conn) -> value_type {
                    shared_ptr<CRedisReply> reply = conn->SendCommand({CRedisCommands::HGET, key_str});
                    return CParam<value_type>(reply->StrValue()).value;
                });
    }

    template<class key_type, class value_type>
    shared_ptr<list<key_type>> RMap<key_type, value_type>::Keys() {
        return KeysAsync().get();
    }

    template<class key_type, class value_type>
    future<shared_ptr<list<key_type>>> RMap<key_type, value_type>::KeysAsync() {
        vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(m_sName)};
        return m_pConnectionManager->GetCommandExecutorService()
                ->RedisAsyncReturnListCommand<key_type>(CRedisCommands::HKEYS, list, m_sName, eCommandModel::READ_ONLY);
    }

    template<class key_type, class value_type>
    shared_ptr<list<pair<key_type, value_type>>> RMap<key_type, value_type>::GetAll() {
        return GetAllAsync().get();
    }

    template<class key_type, class value_type>
    future<shared_ptr<list<pair<key_type, value_type>>>> RMap<key_type, value_type>::GetAllAsync() {
        vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(m_sName)};
        return m_pConnectionManager->GetCommandExecutorService()
                ->RedisAsyncReturnPairListCommand<key_type, value_type>(CRedisCommands::HGETALL,
                                                                        list,
                                                                        m_sName,
                                                                        eCommandModel::READ_ONLY);
    }

    template<class key_type, class value_type>
    shared_ptr<bool> RMap<key_type, value_type>::Exists(const key_type &key) {
        return ExistsAsync(key).get();
    }

    template<class key_type, class value_type>
    future<shared_ptr<bool>> RMap<key_type, value_type>::ExistsAsync(const key_type &key) {
        const string &key_str = ((IRobject * )(&key))->ToString();
        vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(m_sName),
                                              make_shared<CCmdParam>(key_str),
                                              make_shared<CCmdParam>(key_str.length())};
        return m_pConnectionManager->GetCommandExecutorService()
                ->RedisAsyncReturnBoolCommand(CRedisCommands::HEXISTS, list, m_sName, eCommandModel::READ_ONLY);
    }
}
#endif //EXHIREDIS_RMAP_H
