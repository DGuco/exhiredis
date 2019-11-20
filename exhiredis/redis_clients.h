//
// Created by dguco on 18-11-7.
// redis client
//

#ifndef EXHIREDIS_REDIS_CLIENT_HPP
#define EXHIREDIS_REDIS_CLIENT_HPP

#include <memory>
#include <string>
#include <type_traits>
#include "connection_manager.h"
#include "connection_manager.h"
#include "utils/log.h"
#include "rstl/rmap.h"

using namespace std;
namespace exhiredis
{
    class CRedisClients
    {
        public:
            //construct
            CRedisClients(shared_ptr<CConnectionManager> m_pConnectionManager);
            //forbidden operation
            CRedisClients(const CRedisClients &) = delete;
            CRedisClients(const CRedisClients &&) = delete;
            CRedisClients &operator=(CRedisClients &) = delete;
        public:
            /**
             * redis hashes  key_type value_type
             * @tparam key_type key_type must has method FromString(const string &str) and ToString()
             * @tparam value_type value_typee must has method FromString(const string &str) and ToString()
             */
            template<class key_type, class value_type>
            RMap<key_type, value_type> GetMap(const string &name)
            {
                return RMap<key_type, value_type>(name, m_pConnectionManager);
            }

            /**
             *
             * @return RScript executor
             */
            RScript GetScript();
        public:
            static shared_ptr<CRedisClients> CreateInstance(const string& host,int port, const string passwd,int poolSize);
        private:
            shared_ptr<CConnectionManager> m_pConnectionManager;
    };
}

#endif //EXHIREDIS_REDIS_CLIENT_HPP
