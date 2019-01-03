//
// Created by dguco on 18-11-4.
// redis connectionpool
//

#ifndef EXHIREDIS_CONNECTION_POOL_HPP
#define EXHIREDIS_CONNECTION_POOL_HPP

#include <string>
#include <list>
#include "redis_connection.hpp"
#include "connection_manager.hpp"
#include "config/redis_config.hpp"

using namespace std;

namespace exhiredis
{
class CConnectionPool
{
public:
    //construct
    CConnectionPool(const shared_ptr<IConnectionManager>& pConnManager);
    //init connection pool
    void InitPool(int initPoolSize);
    //add one connection
    void AddConnection(shared_ptr<CRedisConnection> conn);
    //borrow one connection
    shared_ptr<CRedisConnection> BorrowOneConnection();
    //back one connection
    shared_ptr<CRedisConnection> BackOneConnection(shared_ptr<CRedisConnection> conn);
private:
    list<shared_ptr<CRedisConnection>> m_connList;
    weak_ptr<IConnectionManager> m_pConnManager;
};

CConnectionPool::CConnectionPool(const shared_ptr<IConnectionManager>& pConnManager)
    : m_pConnManager(pConnManager)
{
    m_connList.clear();
}

void CConnectionPool::InitPool(int initPoolSize)
{
    shared_ptr<CRedisConfig> config = m_pConnManager.lock()->GetRedisConfig();
    for (int i = 0; i < initPoolSize; i++) {
        shared_ptr<CRedisConnection> conn = make_shared<CRedisConnection>();
        conn->ConnecToUnix(config->GetConnConfig()->GetAddress());
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

#endif //EXHIREDIS_CONNECTION_POOL_HPP
