//
// Created by dguco on 19-10-19.
//

#include <mutex>
#include "redis_connection.h"
#include "redis_exception.h"

namespace  exhiredis
{
    CRedisConnection::CRedisConnection()
            :   m_context(nullptr),
                m_sAddressName(""),
                m_sHost(""),
                m_iPort(0),
                m_iTimeOut(0)
    {

    }

    CRedisConnection::~CRedisConnection()
    {
        if (nullptr != m_context)
        {
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

    shared_ptr<CRedisReply> CRedisConnection::SendCommand(const vector<std::string> &commands)
    {
        if (nullptr == m_context)
        {
            throw CRedisException("Redis connection is not inited");
        }

        vector<const char*> argv;
        argv.reserve(commands.size());
        std::vector<size_t> argvlen;
        argvlen.reserve(commands.size());

        for (auto it = commands.begin(); it != commands.end(); ++it) {
            argv.push_back(it->c_str());
            argvlen.push_back(it->size());
        }

        redisReply* reply = static_cast<redisReply *>(redisCommandArgv(m_context, static_cast<int>(commands.size()),
                                                                       argv.data(), argvlen.data()));
        if (nullptr == reply)
        {
            throw CRedisException("Redis redisCommandArgv failed,error: " + m_context->err);
        }

        if (reply->type == REDIS_REPLY_ERROR)
        {
            throw CRedisException("Redis redisCommandArgv failed,msg: " + string(reply->str));
        }

        shared_ptr<CRedisReply> sharedPtr = make_shared<CRedisReply>(reply);
        freeReplyObject(reply);
        return std::move(sharedPtr);
    }

    void CRedisConnection::AppendCommand(const vector<std::string> &commands)
    {
        if (nullptr == m_context)
        {
            throw CRedisException("Redis connection is not inited");
        }

        vector<const char*> argv;
        argv.reserve(commands.size());
        std::vector<size_t> argvlen;
        argvlen.reserve(commands.size());

        for (auto it = commands.begin(); it != commands.end(); ++it) {
            argv.push_back(it->data());
            argvlen.push_back(it->length());
        }

        int ret = redisAppendCommandArgv(m_context, static_cast<int>(commands.size()), argv.data(), argvlen.data());
        if (REDIS_OK != ret)
        {
            throw CRedisException("Redis redisAppendCommandArgv failed,error:" + m_context->err);
        }
        return;
    }

    shared_ptr<CRedisReply> CRedisConnection::GetReply()
    {
        if (nullptr == m_context)
        {
            throw CRedisException("Redis connection is not inited");
        }
        redisReply* reply = nullptr;
        redisGetReply(m_context, reinterpret_cast<void**>(&reply));
        if (nullptr == reply)
        {
            throw CRedisException("Redis redisGetReply failed,error:" + m_context->err);
        }

        if (reply->type == REDIS_REPLY_ERROR)
        {
            throw CRedisException("Redis redisCommandArgv failed,msg: " + string(reply->str));
        }

        shared_ptr<CRedisReply> shareReply = make_shared<CRedisReply>(reply);
        freeReplyObject(reply);

        if (shareReply->ReplyType() == CRedisReply::eReplyType::ERROR &&
            (shareReply->StrValue().find("READONLY") == 0) )
        {
            throw CRedisException("Redis redisGetReply failed,error: slave_read_only");
        }
        return shareReply;
    }

    vector<shared_ptr<CRedisReply>> CRedisConnection::GetReplies(int count)
    {
        std::vector<shared_ptr<CRedisReply>> ret;
        for (int i = 0;i < count; i++)
        {
            ret.push_back(GetReply());
        }
        return ret;
    }
}