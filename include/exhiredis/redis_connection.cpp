//
// Created by dguco on 19-10-19.
//

#include <mutex>
#include "redis_connection.h"

namespace  exhiredis
{
    CRedisConnection::CRedisConnection() :
            m_context(nullptr),
            m_sAddressName(""),
            m_sHost(""),
            m_iPort(0),
            m_iTimeOut(0) {

    }

    CRedisConnection::~CRedisConnection()
    {
        if (nullptr != m_context) {
            redisFree(m_context);
            m_context = nullptr;
        }
    }

    bool CRedisConnection::ConnectWithTimeout(const string &host, int port, int timeout)
    {
        m_sHost = host;
        m_iPort = port;
        m_iTimeOut = timeout;
        struct timeval time;
        time = {timeout, 0};
        m_context = redisConnectWithTimeout(host.c_str(), port, time);
        return m_context != nullptr && m_context->err == REDIS_OK;
    }


    bool CRedisConnection::Connect(const string &host, int port)
    {
        m_sHost = host;
        m_iPort = port;
        m_context = redisConnect(host.c_str(), port);
        return m_context != nullptr && m_context->err == REDIS_OK;
    }


    bool CRedisConnection::ConnectUnixWithTimeout(const string &addressName, int timeout)
    {
        m_sAddressName = addressName;
        m_iTimeOut = timeout;
        struct timeval time;
        time = {timeout, 0};
        m_context = redisConnectUnixWithTimeout(addressName.c_str(), time);
        return m_context != nullptr && m_context->err == REDIS_OK;
    }

    bool CRedisConnection::ConnectUnix(const string &addressName)
    {
        m_context = redisConnectUnix(addressName.c_str());
        return m_context != nullptr && m_context->err == REDIS_OK;
    }
}