//
// Created by dguco on 19-1-4.
//

#include <mutex>
#include "connection_pool.h"
#include "redis_async_connection.h"
#include "redis_exception.h"

namespace exhiredis {

    CConnectionPool::CConnectionPool(const string &host, const string &passwd)
    {
        m_connList.clear();
        this->m_hostName = host;
        this->m_password = passwd;
    }

    void CConnectionPool::InitPool(int initPoolSize)
    {
        std::lock_guard<std::mutex> lock(m_poolLock);
        for (int i = 0;i < initPoolSize; i++)
        {
            shared_ptr<CRedisConnection> conn =  make_shared<CRedisConnection>();
            conn->ConnectUnix(m_hostName);
            m_connList.insert(std::move(conn));
        }
    }

    void CConnectionPool::PutOneCon(shared_ptr<CRedisConnection> conn)
    {
        std::lock_guard<std::mutex> lock(m_poolLock);
        m_connList.insert(conn);
    }

    shared_ptr<CRedisConnection> CConnectionPool::GetOneCon()
    {
        std::lock_guard<std::mutex> lock(m_poolLock);
        if (m_connList.size() <= 0) {
                throw CRedisException("Can't not get free connection from pool");
        }
        auto it = m_connList.begin();
        m_connList.erase(it);
        return *it;
    }
}