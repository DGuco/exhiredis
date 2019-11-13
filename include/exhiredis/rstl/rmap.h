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
    class RMap
    {
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
         * @return  pair<bool,value_type>
         *          left: true ==>key对应的value存在 right key对应的value
         *          left: false ==>key对应的value不存在
         */
        pair<bool,value_type> Get(const key_type &key);

        /**
         * HGET
         * @param key
         * @return  pair<bool,value_type>
         *          left: true ==>key对应的value存在 right key对应的value
         *          left: false ==>key对应的value不存在
         */
        future<pair<bool,value_type>> GetAsync(const key_type &key);

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

        /**
         * HLEN
         * @return map filed size
         */
        int Size();

        /**
         * HLEN
         * @return map filed size
         */
        future<int> SizeAsync();

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
        return m_pConnectionManager->ExecuteCommand<bool>({CRedisCommands::HSET,m_mapName,key_str, value_str});
    }

    template<class key_type, class value_type>
    future<bool> RMap<key_type, value_type>::PutAsync(const key_type &key, const value_type &value)
    {
        const string &key_str = CParam<key_type>(key).ToString();
        const string &value_str = CParam<value_type>(value).ToString();
        return m_pConnectionManager->AsyncExecuteCommand<bool>({CRedisCommands::HSET,m_mapName,key_str, value_str});
    }

    template<class key_type, class value_type>
    pair<bool,value_type> RMap<key_type, value_type>::Get(const key_type &key)
    {
        const string& key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->ExecuteCommand<pair<bool,value_type>>({CRedisCommands::HGET,m_mapName,key_str},
                [](shared_ptr<CRedisReply> reply) -> pair<bool,value_type>
                {
                    CParam<value_type> param;
                    reply->ParseToParam(param);
                    return std::move(std::make_pair(!(reply->ReplyType() == CRedisReply::eReplyType::NIL), param.value));
                });
    }

    template<class key_type, class value_type>
    future<pair<bool,value_type>> RMap<key_type, value_type>::GetAsync(const key_type &key)
    {
        const string& key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->AsyncExecuteCommand<pair<bool,value_type>>({CRedisCommands::HGET,m_mapName,key_str},
                [](shared_ptr<CRedisReply> reply) -> pair<bool,value_type>
                {
                    CParam<value_type> param;
                    reply->ParseToParam(param);
                    return std::move(std::make_pair(!(reply->ReplyType() == CRedisReply::eReplyType::NIL), param.value));
                });
    }

    template<class key_type, class value_type>
    list<key_type> RMap<key_type, value_type>::Keys()
    {
        return m_pConnectionManager->ExecuteCommandReturnList<key_type>({CRedisCommands::HKEYS,m_mapName});
    }

    template<class key_type, class value_type>
    future<list<key_type>> RMap<key_type, value_type>::KeysAsync()
    {
        return m_pConnectionManager->AsyncExecuteCommandReturnList<key_type>({CRedisCommands::HKEYS,m_mapName});
    }

    template<class key_type, class value_type>
    list<pair<key_type, value_type>> RMap<key_type, value_type>::GetAll()
    {
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
                     return std::move(resList);
                 });
    }

    template<class key_type, class value_type>
    future<list<pair<key_type, value_type>>> RMap<key_type, value_type>::GetAllAsync()
    {
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
                    return std::move(resList);
                });
    }

    template<class key_type, class value_type>
    bool RMap<key_type, value_type>::Exists(const key_type &key)
    {
        const string& key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->ExecuteCommand<bool>({CRedisCommands::HEXISTS,m_mapName,key_str});
    }

    template<class key_type, class value_type>
    future<bool> RMap<key_type, value_type>::ExistsAsync(const key_type &key)
    {
        const string& key_str = CParam<key_type>(key).ToString();
        return m_pConnectionManager->AsyncExecuteCommand<bool>({CRedisCommands::HEXISTS,m_mapName,key_str});
    }

    int RMap::Size()
    {
        return m_pConnectionManager->ExecuteCommand<int>({CRedisCommands::HLEN,m_mapName});
    }

    future<int> RMap::SizeAsync()
    {
        return m_pConnectionManager->AsyncExecuteCommand<int>({CRedisCommands::HLEN,m_mapName});
    }
}
#endif //EXHIREDIS_RMAP_H
