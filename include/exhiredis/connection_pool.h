//
// Created by dguco on 18-11-4.
// redis connectionpool
//

#ifndef EXHIREDIS_CONNECTION_POOL_HPP
#define EXHIREDIS_CONNECTION_POOL_HPP

#include <memory>
#include <string>
#include <list>
#include "connection_manager.h"

using namespace std;

namespace exhiredis
{
class CRedisConnection;
class CConnectionPool
{
public:
    //construct
    CConnectionPool(const shared_ptr<IConnectionManager> &pConnManager);
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
}

#endif //EXHIREDIS_CONNECTION_POOL_HPP
