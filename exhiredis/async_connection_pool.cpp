//
// Created by dguco on 19-1-4.
//

#include <mutex>
#include "exhiredis/redis_exception.h"
#include <exhiredis/utils/log.h>
#include "async_connection_pool.h"

namespace exhiredis {

    CAsyncConnectionPool::CAsyncConnectionPool(const string &host,int port,  const string &passwd)
    {
        m_connList.clear();
        this->m_hostName = host;
        this->m_port = port;
        this->m_password = passwd;
    }

    void CAsyncConnectionPool::InitPool(int initPoolSize)
    {
        std::lock_guard<std::mutex> lock(m_poolLock);
        for (int i = 0;i < initPoolSize; i++)
        {
            shared_ptr<CRedisAsyncConnection> conn =  make_shared<CRedisAsyncConnection>();
            if (!conn->Connect(m_hostName,m_port))
            {
                HIREDIS_LOG_ERROR("Connect to redis failed");
            }else
            {
                m_connList.insert(std::move(conn));
            }
        }
    }

    void CAsyncConnectionPool::PutOneCon(shared_ptr<CRedisAsyncConnection> conn)
    {
        std::lock_guard<std::mutex> lock(m_poolLock);
        m_connList.insert(conn);
    }

    shared_ptr<CRedisAsyncConnection> CAsyncConnectionPool::GetOneCon()
    {
        std::lock_guard<std::mutex> lock(m_poolLock);
        if (m_connList.empty()) {
                throw CRedisException("Can't not get free connection from pool");
        }
        auto it = m_connList.begin();
        shared_ptr<CRedisAsyncConnection> conn = *it;
        m_connList.erase(it);
        return conn;
    }

    void CAsyncConnectionPool::Release()
    {
        m_connList.clear();
    }
}