//
// Created by dguco on 19-1-4.
//

#include "redis_clients.h"
#include "exhiredis/single_connection_manager.h"

namespace exhiredis
{
CRedisClients::CRedisClients(shared_ptr<IConnectionManager> &m_pConnectionManager)
    : m_pConnectionManager(move(m_pConnectionManager))
{
    
}

shared_ptr<exhiredis::CRedisClients> CRedisClients::CreateInstance(shared_ptr<CRedisConfig> config)
{
    if (config->GetSingleServerConfig() != nullptr) {
        shared_ptr<IConnectionManager> tmpManager =
            static_pointer_cast<IConnectionManager>(make_shared<CSingleConnectionManager>(config));
        tmpManager->Init();
        return make_shared<CRedisClients>(tmpManager);
    }
    else if (config->GetMasterSlaveConfig()) {
        return shared_ptr<CRedisClients>();
    }

    return nullptr;
}
}