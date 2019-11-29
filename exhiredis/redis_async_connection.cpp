//
// Created by dguco on 19-1-4.
//
#include <event2/thread.h>
#include <hiredis/async.h>
#include "redis_async_connection.h"
#include "redis_exception.h"

namespace exhiredis {

    CRedisAsyncConnection::CRedisAsyncConnection()
            : m_pEventBase(nullptr),
              m_sHost(""),
              m_iPort(0),
              m_connState(enConnState::DEFAULT)
    {

    }

    CRedisAsyncConnection::~CRedisAsyncConnection()
    {
        m_connState = enConnState::DEFAULT;
        ShutDown();
    }

    bool CRedisAsyncConnection::Connect(const string &host, int port, bool isReconn)
    {
        if (!isReconn) {
            m_sHost = host;
            m_iPort = port;
        }

        InitLibevent();
        m_pRedisContext = redisAsyncConnect(m_sHost.c_str(), m_iPort);
        if (nullptr == m_pRedisContext || m_pRedisContext->err) {
            HIREDIS_LOG_ERROR("Redis connect error: %s\n", m_pRedisContext->errstr);
            return false;
        }
        if (!InitHiredis()) {
            return false;
        }

        m_eventLoopThread = std::move(std::thread([this] { RunEventLoop(); }));
        m_connState = enConnState::CONNECTING;
        return true;
    }

    bool CRedisAsyncConnection::ReConnect()
    {
        if (m_pRedisContext != nullptr) {
            redisAsyncFree(m_pRedisContext);
            m_pRedisContext = nullptr;
        }
        Connect(m_sHost,m_iPort,true);
    }

    bool CRedisAsyncConnection::ConnectToUnix(const string &address)
    {
        m_sAddress = address;
        m_pRedisContext = redisAsyncConnectUnix(m_sAddress.c_str());
        if (m_pRedisContext->err) {
            HIREDIS_LOG_ERROR("Redis connect error: %s\n", m_pRedisContext->errstr);
            return false;
        }
        InitLibevent();
        m_eventLoopThread = std::move(std::thread([this] { event_base_dispatch(m_pEventBase); }));
        return true;
    }

    void CRedisAsyncConnection::SendCommandAsync(const vector<std::string> &commands, redisCallbackFn *fn)
    {
        vector<const char *> argv;
        argv.reserve(commands.size());
        std::vector<size_t> argvlen;
        argvlen.reserve(commands.size());

        for (auto it = commands.begin(); it != commands.end(); ++it) {
            argv.push_back(it->c_str());
            argvlen.push_back(it->size());
        }

        int status = redisAsyncCommandArgv(m_pRedisContext, fn, nullptr, static_cast<int>(commands.size()), argv.data(),
                                           argvlen.data());
        if (status != REDIS_OK) {
            throw CRedisException("Redis redisAsyncCommandArgv failed");
        }
    }

    void CRedisAsyncConnection::ShutDown()
    {
        if (m_pRedisContext != nullptr) {
            redisAsyncFree(m_pRedisContext);
            m_pRedisContext = nullptr;
        }
        if (m_pEventBase != nullptr) {
            event_base_loopbreak(m_pEventBase);
            event_base_free(m_pEventBase);
            m_pEventBase = nullptr;
        }
        if (m_eventLoopThread.joinable()) {
            m_eventLoopThread.join();
        }
    }

    bool CRedisAsyncConnection::InitHiredis()
    {
        m_pRedisContext->data = this;
        if (redisLibeventAttach(m_pRedisContext, m_pEventBase) != REDIS_OK) {
            HIREDIS_LOG_ERROR("redisLibeventAttach failed\n");
            return false;
        }
        if (redisAsyncSetConnectCallback(m_pRedisContext, &CRedisAsyncConnection::lcb_OnConnectCallback) != REDIS_OK) {
            HIREDIS_LOG_ERROR("redisAsyncSetConnectCallback failed\n");
            return false;
        }
        if (redisAsyncSetDisconnectCallback(m_pRedisContext, &CRedisAsyncConnection::lcb_OnDisconnectCallback) !=
            REDIS_OK) {
            HIREDIS_LOG_ERROR("redisAsyncSetDisconnectCallback failed\n");
            return false;
        }
        return true;
    }

    void CRedisAsyncConnection::InitLibevent()
    {
#ifdef __linux__
        int ret = evthread_use_pthreads();
        if (ret != 0) {
            HIREDIS_LOG_ERROR("evthread_use_pthreads on event base failed\n");
        }
#elif _WIN32 || _WIN64
        int ret = evthread_use_windows_threads();
        if (ret != 0)
        {
            HIREDIS_LOG_ERROR("evthread_use_pthreads on event base failed\n");
        }
#endif


        m_pEventBase = event_base_new();
        if (nullptr == m_pEventBase) {
            throw CRedisException("Create event base error\n");
        }
    }

    bool CRedisAsyncConnection::RunEventLoop()
    {
        ignore_pipe();
        event_base_dispatch(m_pEventBase);
        return true;
    }

    void CRedisAsyncConnection::SetConnState(enConnState m_connState)
    {
        CRedisAsyncConnection::m_connState = m_connState;
    }


    enConnState CRedisAsyncConnection::GetConnState()
    {
        return m_connState;
    }

    void CRedisAsyncConnection::lcb_OnConnectCallback(const redisAsyncContext *context, int status)
    {
        CRedisAsyncConnection *conn = (CRedisAsyncConnection *) context->data;
        if (status != REDIS_OK) {
            conn->SetConnState(enConnState::CONNECTED_ERROR);
            HIREDIS_LOG_ERROR("Could not connect to redis,error msg: %s,status: %d", context->errstr, status);
        } else {
//        HIREDIS_LOG_INFO("Connected to Redis succeed.");
//        //禁止hiredis 自动释放reply
//        context->c.reader->fn->freeObject = [](void *reply)
//        {};
            conn->SetConnState(enConnState::CONNECTED);
        }
    }

    void CRedisAsyncConnection::lcb_OnDisconnectCallback(const redisAsyncContext *context, int status)
    {
        CRedisAsyncConnection *conn = (CRedisAsyncConnection *) context->data;
        if (conn->GetConnState() == enConnState::DEFAULT) {
            return;
        }
        conn->SetConnState(enConnState::DISCONNECTED);
        conn->ReConnect();
    }

    void CRedisAsyncConnection::lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata)
    {
        CRedisAsyncConnection *conn = (CRedisAsyncConnection *) context->data;
        unsigned long cmdId = (unsigned long) privdata;
        redisReply *cmdReply = (redisReply *) reply;
    }
}