//
// Created by dguco on 18-11-7.
// redis client
//

#ifndef EXHIREDIS_REDIS_CLIENT_HPP
#define EXHIREDIS_REDIS_CLIENT_HPP

#include <memory>
#include <string>
#include <type_traits>
#include "connection_manager.hpp"
#include "rstl/rmap.hpp"
#include "rstl/rscript.hpp"
#include "utils/log.hpp"
#include "master_slave_manager.hpp"

using namespace std;

namespace exhiredis
{
class CRedisClients
{
private:
    //construct
    CRedisClients(shared_ptr<IConnectionManager> m_pConnectionManager);
    //forbidden operation
    CRedisClients(const CRedisClients &) = delete;
    CRedisClients(const CRedisClients &&) = delete;
    CRedisClients &operator=(CRedisClients &) = delete;
public:
    /**
     *
     * @tparam key_type key_type must be the subclass of the IRobject,you can inherit the class Robject ,ex:RBool,RInt,RFloat......
     * @tparam value_type value_type must be the subclass of the IRobject,you can inherit the class Robject ex:RBool,RInt,RFloat......
     * @param   name hashes name
     * @return  redis hashes  key_type value_type (必须实现IRobject接口,你可以实现Robject接口，例如:RBool,RInt,RFloat......)
     */
    template<class key_type, class value_type>
    shared_ptr<RMap<key_type, value_type>> GetMap(const string &name);
public:
    static shared_ptr<CRedisClients> CreateInstance(shared_ptr<CRedisConfig> config);
private:
    shared_ptr<IConnectionManager> m_pConnectionManager;
};

CRedisClients::CRedisClients(shared_ptr<IConnectionManager> m_pConnectionManager)
    : m_pConnectionManager(move(m_pConnectionManager))
{
    CLog::CreateInstance();
}

shared_ptr<exhiredis::CRedisClients> CRedisClients::CreateInstance(shared_ptr<CRedisConfig> config)
{
    if (config->GetSingleServerConfig() != nullptr) {
        shared_ptr<IConnectionManager> tmpManager = make_shared<CMasterSlaveManager>(config);
        return make_shared<CRedisClients>(tmpManager);
    }
    else if (config->GetMasterSlaveConfig()) {
        return shared_ptr<CRedisClients>();
    }

    return nullptr;
}

template<class key_type, class value_type>
shared_ptr<RMap<key_type, value_type>> CRedisClients::GetMap(const string &name)
{
    if (!is_base_of<IRobject, key_type>::value || !is_base_of<IRobject, value_type>::value) {
        HIREDIS_LOG_ERROR("The key_type and value_type must be subclass of the IRobject");
        return nullptr;
    }
    return make_shared<RMap<key_type, value_type>>(name, m_pConnectionManager);
}
}

#endif //EXHIREDIS_REDIS_CLIENT_HPP