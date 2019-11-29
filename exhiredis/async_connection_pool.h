//
// Created by dguco on 18-11-4.
// redis connectionpool
//

#ifndef EXHIREDIS_ASYNC_CONNECTION_POOL_HPP
#define EXHIREDIS_ASYNC_CONNECTION_POOL_HPP

#include <memory>
#include <string>
#include <set>
#include <mutex>
#include "redis_async_connection.h"

using namespace std;

namespace exhiredis
{
class CAsyncConnectionPool
{
public:
    //construct
    CAsyncConnectionPool(const string &host,int port, const string &passwd);
    //init connection pool
    void InitPool(int initPoolSize);
    //add one connection
    void PutOneCon(shared_ptr<CRedisAsyncConnection> conn);
    //borrow one connection
    shared_ptr<CRedisAsyncConnection> GetOneCon();
    //Release
    void Release();
private:
    set<shared_ptr<CRedisAsyncConnection>> m_connList;
    mutex m_poolLock;
    string m_hostName;
    int m_port;
    string m_password;
    int m_poolSize;
    unsigned int m_database;
};
}

#endif //EXHIREDIS_ASYNC_CONNECTION_POOL_HPP
