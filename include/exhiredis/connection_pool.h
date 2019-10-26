//
// Created by dguco on 18-11-4.
// redis connectionpool
//

#ifndef EXHIREDIS_CONNECTION_POOL_HPP
#define EXHIREDIS_CONNECTION_POOL_HPP

#include <memory>
#include <string>
#include <list>
#include <mutex>
#include "redis_connection.h"
#include "connection_manager.h"

using namespace std;

namespace exhiredis
{
class CRedisC;
class CConnectionPool
{
public:
    //construct
    CConnectionPool();
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
    mutex m_poolLock;
    list<string> m_sentinelHosts;
    string m_masterName;
    string m_password;
    unsigned int m_database;
};
}

#endif //EXHIREDIS_CONNECTION_POOL_HPP
