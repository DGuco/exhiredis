//
// Created by dguco on 19-1-4.
//
#include "connection_manager.h"

namespace exhiredis
{

    CConnectionManager::CConnectionManager(): m_pConnectionPool(nullptr) {

    }

    void CConnectionManager::Init(const string &host,int port, const string &passwd,int poolSize,int asyncPoolSize)
    {
        m_pConnectionPool = make_shared<CConnectionPool>(host,port,passwd);
        m_pConnectionPool->InitPool(poolSize);
        m_pAsyncConnPool = make_shared<CAsyncConnectionPool>(host,port,passwd);
        m_pAsyncConnPool->InitPool(asyncPoolSize);
    }

    void CConnectionManager::PutOneCon(shared_ptr<CRedisConnection> conn)
    {
        m_pConnectionPool->PutOneCon(conn);
    }

    shared_ptr<CRedisConnection> CConnectionManager::GetOneCon()
    {
        if (nullptr == m_pConnectionPool)
            throw CRedisException("ConnectionPool is null");
        return m_pConnectionPool->GetOneCon();
    }
}