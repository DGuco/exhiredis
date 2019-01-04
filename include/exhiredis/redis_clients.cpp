//
// Created by dguco on 19-1-4.
//

#include "redis_clients.h"
#include "exhiredis/exhiredis_include.h"

namespace exhiredis
{
CRedisClients::CRedisClients(shared_ptr<IConnectionManager> &m_pConnectionManager)
    : m_pConnectionManager(move(m_pConnectionManager))
{
}

shared_ptr<exhiredis::CRedisClients> CRedisClients::CreateInstance(shared_ptr<CRedisConfig> config)
{
    if (config->GetSingleServerConfig() != nullptr) {
        shared_ptr<IConnectionManager> tmpManager
            = static_pointer_cast<IConnectionManager>(make_shared<CSingleConnectionManager>(config));
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

shared_ptr<IConnectionManager> &CRedisClients::GetConnectionManager()
{
    return m_pConnectionManager;
}
}