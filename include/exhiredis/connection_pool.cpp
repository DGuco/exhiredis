//
// Created by dguco on 19-1-4.
//

#include <atomic>
#include "connection_pool.h"
#include "redis_connection.h"
#include "connection_manager.h"
#include "exhiredis/config/redis_config.h"

namespace exhiredis
{
CConnectionPool::CConnectionPool(const shared_ptr<IConnectionManager> &pConnManager)
    : m_pConnManager(pConnManager)
{
    m_connList.clear();
    atomic<int> atomic1;
    atomic1.compare_exchange_strong()
}

void CConnectionPool::InitPool(int initPoolSize)
{
    shared_ptr<CRedisConfig> config = m_pConnManager.lock()->GetRedisConfig();
    for (int i = 0; i < initPoolSize; i++) {
        shared_ptr<CRedisConnection> conn = make_shared<CRedisConnection>();
        conn->ConnectToUnix(config->GetConnConfig()->GetAddress());
    }
}

void CConnectionPool::AddConnection(shared_ptr<CRedisConnection> conn)
{
    m_connList.push_back(move(conn));
}

shared_ptr<CRedisConnection> CConnectionPool::BorrowOneConnection()
{
    if (m_connList.size() > 0) {
        shared_ptr<CRedisConnection> &it = m_connList.front();
        m_connList.pop_front();
        return it;
    }
    return nullptr;
}

shared_ptr<CRedisConnection> CConnectionPool::BackOneConnection(shared_ptr<CRedisConnection> conn)
{
    m_connList.push_back(move(conn));
}
}
