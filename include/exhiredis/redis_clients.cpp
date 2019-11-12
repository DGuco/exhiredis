//
// Created by dguco on 19-1-4.
//

#include "redis_clients.h"

namespace exhiredis
{
    CRedisClients::CRedisClients(shared_ptr<CConnectionManager> connManager)
            : m_pConnectionManager(connManager)
    {

    }

    shared_ptr<exhiredis::CRedisClients> CRedisClients::CreateInstance(const string& host,int port,const string passwd,int poolSize)
    {
        shared_ptr<CConnectionManager> conManager = make_shared<CConnectionManager>();
        conManager->Init(host,port,passwd,poolSize);
        return make_shared<CRedisClients>(std::move(conManager));
    }
}