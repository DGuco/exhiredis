//
// Created by dguco on 19-1-4.
//

#include "redis_clients.h"

namespace exhiredis
{
    CRedisClients::CRedisClients() : m_pConnectionManager(nullptr)
    {

    }

    shared_ptr<exhiredis::CRedisClients> CRedisClients::CreateInstance()
    {
        return nullptr;
    }
}