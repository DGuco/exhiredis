//
// Created by dguco on 19-1-4.
//

#include <atomic>
#include "connection_pool.h"
#include "redis_async_connection.h"

namespace exhiredis
{
CConnectionPool::CConnectionPool()
{
    m_connList.clear();
}

void CConnectionPool::InitPool(int initPoolSize)
{

}

void CConnectionPool::AddConnection(shared_ptr<CRedisConnection> conn)
{

}

shared_ptr<CRedisConnection> CConnectionPool::BorrowOneConnection()
{
}

shared_ptr<CRedisConnection> CConnectionPool::BackOneConnection(shared_ptr<CRedisConnection> conn)
{
    m_connList.push_back(move(conn));
}
}
