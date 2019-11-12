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
     * redis hashes  key_type value_type
     * @tparam key_type key_type must has method FromString(const string &str) and ToString()
     * @tparam value_type value_typee must has method FromString(const string &str) and ToString()
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
        value_type Get(const key_type &key);

        /**
         * async HGET
         * @param key
         * @return
         */
        future<value_type> GetAsync(const key_type &key);

        /**
        * HKEYS
        * @return all map keys
        */
        list<key_type> Keys();

        /**
         * async HKEYS
         * @return
         */
        future<list<key_type>> KeysAsync();

        /**
         * HGETALL
         * @return list<pair<key_type, value_type>>
         */
        list<pair<key_type, value_type>> GetAll();

        /**
         * async HGETALL
         * @return list<pair<key_type, value_type>>
         */
        future<list<pair<key_type, value_type>>> GetAllAsync();

        /**
         * HEXISTS
         * @param key
         * @return true or false
         */
        bool Exists(const key_type &key);

        /**
         * async HEXISTS
         * @param key
         * @return true or false
         */
        future<bool> ExistsAsync(const key_type &key);

    private:
        string m_mapName;
        shared_ptr<CConnectionManager> m_pConnectionManager;
    };

    template<class key_type, class value_type>
    RMap<key_type, value_type>::RMap(const string &name, shared_ptr<CConnectionManager> &conn)
            : m_mapName(name),
              m_pConnectionManager(conn)
    {}

    template<class key_type, class value_type>
    bool RMap<key_type, value_type>::Put(const key_type &key, const value_type &value)
    {
        const string &key_str = CParam<key_type>(key).ToString();
        const string &value_str = CParam<value_type>(value).ToString();
        return m_pConnectionManager->ExecuteCommand<bool>({CRedisCommands::HSET, key_str, value_str});
    }

    template<class key_type, class value_type>
    future<bool> RMap<key_type, value_type>::PutAsync(const key_type &key, const value_type &value) {
        const string &key_str = CParam<key_type>(key).ToString();
        const string &value_str = CParam<value_type>(value).ToString();
        return m_pConnectionManager->AsyncExecuteCommand<bool>({CRedisCommands::HSET, key_str, value_str});
    }

    template<class key_type, class value_type>
    value_type RMap<key_type, value_type>::Get(const key_type &key) {
        const string& key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->ExecuteCommand<value_type>({CRedisCommands::HGET, key_str});
    }

    template<class key_type, class value_type>
    future<value_type> RMap<key_type, value_type>::GetAsync(const key_type &key) {
        const string& key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->AsyncExecuteCommand<value_type>({CRedisCommands::HGET, key_str});
    }

    template<class key_type, class value_type>
    list<key_type> RMap<key_type, value_type>::Keys() {
        return m_pConnectionManager->ExecuteCommandReturnList<key_type>({CRedisCommands::HKEYS,m_mapName});
    }

    template<class key_type, class value_type>
    future<list<key_type>> RMap<key_type, value_type>::KeysAsync() {
        return m_pConnectionManager->AsyncExecuteCommandReturnList<key_type>({CRedisCommands::HKEYS,m_mapName});
    }

    template<class key_type, class value_type>
    list<pair<key_type, value_type>> RMap<key_type, value_type>::GetAll() {
        return m_pConnectionManager->ExecuteCommandReturnList<pair<key_type, value_type>>
                ({CRedisCommands::HGETALL,m_mapName},
                 [](shared_ptr<CRedisReply> reply) -> list<pair<key_type, value_type>>
                 {
                     list<pair<key_type, value_type>> resList;
                     for (size_t i = 0; i < reply->ArrayElements().size() - 1;i += 2)
                     {
                         const CRedisReply& keyReply = reply->ArrayElements().at(i);
                         const CRedisReply& valueReply = reply->ArrayElements().at(i);
                         CParam<key_type> keyParam;
                         keyReply.ParseToParam(keyParam);
                         CParam<value_type> valueParam;
                         valueReply.ParseToParam(valueParam);
                         resList.push_back(std::make_pair(keyParam.value,valueParam.value));
                     }
                     return resList;
                 });
    }

    template<class key_type, class value_type>
    future<list<pair<key_type, value_type>>> RMap<key_type, value_type>::GetAllAsync() {
        return m_pConnectionManager->AsyncExecuteCommandReturnList<pair<key_type, value_type>>
                ({CRedisCommands::HGETALL,m_mapName},
                [](shared_ptr<CRedisReply> reply) -> list<pair<key_type, value_type>>
                {
                    list<pair<key_type, value_type>> resList;
                    for (size_t i = 0; i < reply->ArrayElements().size() - 1;i += 2)
                    {
                        const CRedisReply& keyReply = reply->ArrayElements().at(i);
                        const CRedisReply& valueReply = reply->ArrayElements().at(i);
                        CParam<key_type> keyParam;
                        keyReply.ParseToParam(keyParam);
                        CParam<value_type> valueParam;
                        valueReply.ParseToParam(valueParam);
                        resList.push_back(std::make_pair(keyParam.value,valueParam.value));
                    }
                    return resList;
                });
    }

    template<class key_type, class value_type>
    bool RMap<key_type, value_type>::Exists(const key_type &key) {
        const string& key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->ExecuteCommand<bool>({CRedisCommands::HEXISTS,m_mapName,key_str});
    }

    template<class key_type, class value_type>
    future<bool> RMap<key_type, value_type>::ExistsAsync(const key_type &key) {
        const string& key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->AsyncExecuteCommand<bool>({CRedisCommands::HEXISTS,m_mapName,key_str});
    }
}
#endif //EXHIREDIS_RMAP_H
