//
// Created by dguco on 19-1-4.
//

#include <mutex>
#include "exhiredis/redis_exception.h"
#include <exhiredis/utils/log.h>
#include "connection_pool.h"

namespace exhiredis {

    CConnectionPool::CConnectionPool(const string &host,int port,  const string &passwd)
    {
        m_connList.clear();
        this->m_hostName = host;
        this->m_port = port;
        this->m_password = passwd;
    }

    void CConnectionPool::InitPool(int initPoolSize)
    {
        std::lock_guard<std::mutex> lock(m_poolLock);
        for (int i = 0;i < initPoolSize; i++)
        {
            shared_ptr<CRedisConnection> conn =  make_shared<CRedisConnection>();
            if (!conn->Connect(m_hostName,m_port))
            {
                HIREDIS_LOG_ERROR("Connect to redis failed");
            }else
            {
                m_connList.insert(std::move(conn));
            }
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
        if (m_connList.empty()) {
                throw CRedisException("Can't not get free connection from pool");
        }
        auto it = m_connList.begin();
        shared_ptr<CRedisConnection> conn = *it;
        m_connList.erase(it);
        return conn;
    }
}