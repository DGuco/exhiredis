//
// Created by dguco on 18-11-4.
// redis connectionpool
//

#ifndef EXHIREDIS_CONNECTION_POOL_HPP
#define EXHIREDIS_CONNECTION_POOL_HPP

#include <memory>
#include <string>
#include <set>
#include <mutex>
#include "redis_connection.h"

using namespace std;

namespace exhiredis
{
class CConnectionPool
{
public:
    //construct
    CConnectionPool(const string &host,int port, const string &passwd);
    //init connection pool
    void InitPool(int initPoolSize);
    //add one connection
    void PutOneCon(shared_ptr<CRedisConnection> conn);
    //borrow one connection
    shared_ptr<CRedisConnection> GetOneCon();
    //Release
    void Release();
private:
    set<shared_ptr<CRedisConnection>> m_connList;
    mutex m_poolLock;
    string m_hostName;
    int m_port;
    string m_password;
    int m_poolSize;
    unsigned int m_database;
};
}

#endif //EXHIREDIS_CONNECTION_POOL_HPP
