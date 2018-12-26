//
// Created by dguco on 18-10-17.
// redis connection
//

#ifndef EXHIREDIS_REDIS_CONN_H
#define EXHIREDIS_REDIS_CONN_H
#include <hiredis/adapters/libevent.h>
#include <hiredis/async.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <event.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>
#include <unordered_map>
#include "utils/signal.hpp"
#include "utils/log.hpp"
#include "command.hpp"
#include "utils/uuid.hpp"
#include "robject/rint.hpp"

namespace exhiredis
{
class CRedisConnection
{
public:
    enum enConnState
    {
        DESTROYING = -1,       //call the ~CRedisConnection 析构函数
        NOT_YET_CONNECTED = 0, // Starting state
        CONNECTING = 1,        // connecting
        CONNECTED = 2,        // Successfully connected
        DISCONNECTED = 3,      // Successfully disconnected
        CONNECT_ERROR = 4,     // Error connecting
        INIT_ERROR = 5,        // Failed to init data structures
    };
public:
    //构造函数
    CRedisConnection();
    CRedisConnection(const CRedisConnection &tmp) = delete;
    CRedisConnection(CRedisConnection &&tmp) = delete;
    //析构函数
    virtual ~CRedisConnection();
    //connect redis
    bool Connect(const string &host, int portn, bool isReconn = false);
    //reconnect to redis
    bool ReConnect();
    //connect redis
    bool ConnecToUnix(const string &address);
    //get status of the connecton
    enConnState GetConnState();
    //set status of the connecton
    void SetConnState(enConnState eConnState);
    //execute redis command ,don't care what redis returns,only care whether the command succeed or not
    future<bool> RedisAsyncIsSucceedCommand(const char *cmd, ...);
    //execute redis command return integer pointer or nullptr
    future<shared_ptr<long long>> RedisAsyncReturnIntCommand(const char *cmd, ...);
    //execute redis command return bool pointer or nullptr
    future<shared_ptr<bool>> RedisAsyncReturnBoolCommand(const char *cmd, ...);
    //execute redis command return string pointer or nullptr
    future<shared_ptr<string>> RedisAsyncReturnStringCommand(const char *cmd, ...);
    //execute redis command return bool pointer or nullptr,return_type must be the subclass of the IRobject
    template<class return_type>
    future<shared_ptr<return_type>> RedisAsyncCommand(const char *cmd, ...);
private:
    //create cmd
    shared_ptr<CCommand> CreateCommand(const char *cmd, va_list vaList);
    //execute redis
    shared_ptr<CCommand> RedisvAsyncCommand(const char *cmd, va_list ap);
    //execute redis
    int SendCommandAsync(shared_ptr<CCommand> &command);
    //create command id
    unsigned long GenCommandId();
    //shutdown the connection
    void ShutDown();
    //flush all command
    void RetryFailedOrWaitCommands();
private:
    //connected callback
    static void lcb_OnConnectCallback(const redisAsyncContext *context, int status);
    //disconnected callback(this callback will not call until we found the connection is gone when we send failed or we closed ourselves)
    static void lcb_OnDisconnectCallback(const redisAsyncContext *context, int status);
    //the server closed our connection
    static void lcb_OnDisconnectEventCallback(int socket, int event, void *data);
    //redis command callback
    static void lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata);
private:
    //init hiredis
    bool InitHiredis();
    //init libevent
    bool InitLibevent();
    //run eventloop
    bool RunEventLoop();
    //set command reply
    void SetCommandReply(unsigned long cmdId, redisReply *cmdReply);
private:
    redisAsyncContext *m_pRedisContext;
    shared_ptr<thread> m_pEventLoopThread;
    event_base *m_pEventBase;
    event *m_closedEvent;
    string m_sHost;
    int m_iPort;
    string m_sAddress;
    enConnState m_connState;
    mutex m_lock;
    atomic_ulong m_cmdId;
    unordered_map<unsigned long, shared_ptr<CCommand>> m_cmdMap;
    queue<unsigned long> m_cmdQueue;
    list<unsigned long> m_failedOrWaitList;
};

CRedisConnection::CRedisConnection()
    : m_pEventLoopThread(nullptr),
      m_pEventBase(nullptr),
      m_sHost(""),
      m_iPort(0),
      m_connState(enConnState::NOT_YET_CONNECTED)
{

}

CRedisConnection::~CRedisConnection()
{
    SetConnState(enConnState::DESTROYING);
    ShutDown();
}

bool CRedisConnection::Connect(const string &host, int port, bool isReconn)
{
    if (!isReconn) {
        m_sHost = host;
        m_iPort = port;
    }

    if (!InitLibevent()) {
        SetConnState(enConnState::INIT_ERROR);
        return false;
    }
    m_pRedisContext = redisAsyncConnect(m_sHost.c_str(), m_iPort);
    if (nullptr == m_pRedisContext || m_pRedisContext->err) {
        HIREDIS_LOG_ERROR("Redis connect error: %s\n", m_pRedisContext->errstr);
        SetConnState(enConnState::INIT_ERROR);
        return false;
    }
    if (!InitHiredis()) {
        return false;
    }
    SetConnState(enConnState::CONNECTING);
    if (m_pEventLoopThread != nullptr) {
        m_pEventLoopThread.reset();
    }
    m_pEventLoopThread = make_shared<thread>([this]
                                             { RunEventLoop(); });
    return GetConnState() == enConnState::CONNECTED;
}

bool CRedisConnection::ReConnect()
{
    {
        lock_guard<mutex> lock_guard(m_lock);
        if (GetConnState() == enConnState::DISCONNECTED) {
            ShutDown();
            Connect(m_sHost, m_iPort, true);
        }
    }
    return GetConnState() == enConnState::CONNECTED;
}

bool CRedisConnection::ConnecToUnix(const string &address)
{
    m_sAddress = address;
    m_pRedisContext = redisAsyncConnectUnix(m_sAddress.c_str());
    if (m_pRedisContext->err) {
        HIREDIS_LOG_ERROR("Redis connect error: %s\n", m_pRedisContext->errstr);
        return false;
    }
    m_pEventBase = event_base_new();
    if (nullptr == m_pEventBase) {
        HIREDIS_LOG_ERROR("Create event base error\n");
    }
    m_pEventLoopThread = make_shared<thread>([this]
                                             { event_base_dispatch(m_pEventBase); });
    return GetConnState() == enConnState::CONNECTED;
}

enConnState CRedisConnection::GetConnState()
{
    lock_guard<mutex> lk(m_lock);
    return m_connState;
}

void CRedisConnection::SetConnState(enConnState eConnState)
{
    lock_guard<mutex> lk(m_lock);
    m_connState = eConnState;
}

shared_ptr<CCommand> CRedisConnection::RedisvAsyncCommand(const char *cmd,
                                                          va_list ap)
{
    shared_ptr<CCommand> tmpCommand = CreateCommand(cmd, ap);
    lock_guard<mutex> lk(m_lock);
    unsigned long commandId = tmpCommand->GetCommandId();
    m_cmdMap.insert(make_pair(commandId, tmpCommand));
    m_cmdQueue.push(commandId);
    if (m_connState == enConnState::CONNECTED) {
        //retry failed or commands
        RetryFailedOrWaitCommands();
        if (SendCommandAsync(tmpCommand) != REDIS_OK) {
            tmpCommand->SetCommState(CCommandState::SEND_ERROR);
            m_failedOrWaitList.push_back(commandId);
            HIREDIS_LOG_ERROR("Send redis command failed,cmd = %s", tmpCommand->ToString());
        }
    }
    else {
        m_failedOrWaitList.push_back(commandId);
    }
    return std::move(tmpCommand);
}

int CRedisConnection::SendCommandAsync(shared_ptr<CCommand> &command)
{
    int status = redisvAsyncCommand(m_pRedisContext,
                                    &CRedisConnection::lcb_OnCommandCallback,
                                    (void *) command->GetCommandId(),
                                    command->GetCmd(),
                                    command->GetParam());
    if (status == REDIS_OK) {
        command->SetCommState(CCommandState::NO_REPLY_YET);
    }
    return status;
}

future<bool> CRedisConnection::RedisAsyncIsSucceedCommand(const char *cmd, ...)
{
    va_list ap;
    va_start(ap, cmd);
    shared_ptr<CCommand> command = RedisvAsyncCommand(cmd, ap);
    va_end(ap);
    return async([command]() -> bool
                 {
                     redisReply *res = command->GetPromise()->get_future().get();
                     if (res == nullptr) {
                         return false;
                     }
                     if (res->type == REDIS_REPLY_ERROR) {
                         HIREDIS_LOG_ERROR("Redis reply error,error msg: %s", res->str);
                         return false;
                     }

                     if (res->type == REDIS_REPLY_STATUS) {
                         return strcmp(res->str, "OK") == 0;
                     }
                     //don't care what the result is
                     return true;
                 });
}

future<shared_ptr<long long>> CRedisConnection::RedisAsyncReturnIntCommand(const char *cmd, ...)
{
    va_list ap;
    va_start(ap, cmd);
    shared_ptr<CCommand> command = RedisvAsyncCommand(cmd, ap);
    va_end(ap);
    return async([command]() -> shared_ptr<long long>
                 {
                     redisReply *res = command->GetPromise()->get_future().get();
                     if (res == nullptr) {
                         return nullptr;
                     }
                     if (res->type == REDIS_REPLY_ERROR) {
                         HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                         return nullptr;
                     }

                     if (res->type == REDIS_REPLY_INTEGER) {
                         return make_shared<long long>(res->integer);
                     }
                     return nullptr;
                 });
}

future<shared_ptr<bool>> CRedisConnection::RedisAsyncReturnBoolCommand(const char *cmd, ...)
{
    va_list ap;
    va_start(ap, cmd);
    shared_ptr<CCommand> command = RedisvAsyncCommand(cmd, ap);
    va_end(ap);
    return async([command]() -> shared_ptr<bool>
                 {
                     redisReply *res = command->GetPromise()->get_future().get();
                     if (res == nullptr) {
                         return nullptr;
                     }
                     if (res->type == REDIS_REPLY_ERROR) {
                         HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                         return nullptr;
                     }

                     /*
                        *Lua boolean false -> Redis Nil bulk reply / Lua 的布尔值 false 转换成 Redis 的 Nil bulk 回复
                        *Lua boolean true -> Redis integer reply with value of 1 / Lua 布尔值 true 转换成 Redis 整数回复中的 1
                       **/
                     if (res->type == REDIS_REPLY_NIL) {
                         return make_shared<bool>(false);
                     }
                     else if (res->type == REDIS_REPLY_INTEGER && res->integer == 1) {
                         return make_shared<bool>(true);
                     }
                     else {
                         return nullptr;
                     }
                 });
}

future<shared_ptr<string>> CRedisConnection::RedisAsyncReturnStringCommand(const char *cmd, ...)
{
    va_list ap;
    va_start(ap, cmd);
    shared_ptr<CCommand> tmpCommand = RedisvAsyncCommand(cmd, ap);
    va_end(ap);
    return async([tmpCommand]() -> shared_ptr<string>
                 {
                     future<redisReply *> future = tmpCommand->GetPromise()->get_future();
                     redisReply *res = future.get();
                     if (res == nullptr) {
                         return nullptr;
                     }

                     if (res->type == REDIS_REPLY_ERROR) {
                         HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                         return nullptr;
                     }

                     if (res->type == REDIS_REPLY_INTEGER) {
                         string resValue;
                         shared_ptr<string> value = make_shared<string>(to_string(res->integer));
                         return value;
                     }

                     if (res->type == REDIS_REPLY_STRING) {
                         string resValue;
                         shared_ptr<string> value = make_shared<string>(resValue.assign(res->str, res->len));
                         return value;
                     }
                     return nullptr;
                 });
}

template<class return_type>
future<shared_ptr<return_type>> CRedisConnection::RedisAsyncCommand(const char *cmd, ...)
{
    va_list ap;
    va_start(ap, cmd);
    shared_ptr<CCommand> tmpCommand = RedisvAsyncCommand(cmd, ap);
    va_end(ap);
    return async([tmpCommand]() -> shared_ptr<return_type>
                 {
                     redisReply *res = tmpCommand->GetPromise()->get_future().get();
                     if (res == nullptr) {
                         return nullptr;
                     }

                     if (res->type == REDIS_REPLY_ERROR) {
                         HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                         return nullptr;
                     }

                     if (res->type == REDIS_REPLY_NIL) {
                         return nullptr;
                     }

                     if (res->type == REDIS_REPLY_STRING) {
                         shared_ptr<return_type> value = make_shared<return_type>();
                         static_pointer_cast<IRobject>(value)->FromString(res->str, res->len);
                         return value;
                     }
                     return nullptr;
                 });
}

void CRedisConnection::ShutDown()
{
    if (m_closedEvent != nullptr) {
        event_free(m_closedEvent);
    }
    if (m_pRedisContext != nullptr) {
        redisAsyncFree(m_pRedisContext);
    }
    if (m_pEventBase != nullptr) {
        event_base_loopbreak(m_pEventBase);
        event_base_free(m_pEventBase);
    }
    if (m_pEventLoopThread != nullptr) {
        if (m_pEventLoopThread->joinable()) {
            m_pEventLoopThread->join();
        }
    }
}

void CRedisConnection::RetryFailedOrWaitCommands()
{
    if (m_failedOrWaitList.size() > 0) {
        list<unsigned long> tmpList = m_failedOrWaitList;
        m_failedOrWaitList.clear();
        while (tmpList.size() > 0) {
            auto it = m_cmdMap.find(tmpList.front());
            if (it != m_cmdMap.end()) {
                shared_ptr<CCommand> &command = it->second;
                int status = SendCommandAsync(command);
                if (status != REDIS_OK && command->GetCommState() == CCommandState::SEND_ERROR) {
                    HIREDIS_LOG_ERROR("Retry command failed,command = %s", command->ToString());
                }
            }
        }
    }
}

void CRedisConnection::lcb_OnConnectCallback(const redisAsyncContext *context, int status)
{
    CRedisConnection *conn = (CRedisConnection *) context->data;
    if (status != REDIS_OK) {
        HIREDIS_LOG_ERROR("Could not connect to redis,error msg: %s,status: %d", context->errstr, status);
        conn->SetConnState(enConnState::CONNECT_ERROR);
    }
    else {
        HIREDIS_LOG_INFO("Connected to Redis succeed.");
        //禁止hiredis 自动释放reply
        context->c.reader->fn->freeObject = [](void *reply)
        {};
        conn->SetConnState(enConnState::CONNECTED);
    }
}

void CRedisConnection::lcb_OnDisconnectCallback(const redisAsyncContext *context, int status)
{
    CRedisConnection *conn = (CRedisConnection *) context->data;
    if (conn == nullptr || conn->GetConnState() == enConnState::DESTROYING) {
        return;
    }
    if (conn != nullptr) {
        conn->SetConnState(enConnState::DISCONNECTED);
    }

    HIREDIS_LOG_ERROR("Disconnected from redis");
    conn->ReConnect();
}

void CRedisConnection::lcb_OnDisconnectEventCallback(int socket, int event, void *data)
{
    CRedisConnection *conn = (CRedisConnection *) data;
    if (conn != nullptr) {
        conn->SetConnState(enConnState::DISCONNECTED);
        HIREDIS_LOG_ERROR("Disconnected from redis");
        conn->ReConnect();
    }
}

void CRedisConnection::lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata)
{
    CRedisConnection *conn = (CRedisConnection *) context->data;
    unsigned long cmdId = (unsigned long) privdata;
    redisReply *cmdReply = (redisReply *) reply;
    conn->SetCommandReply(cmdId, cmdReply);
}

void CRedisConnection::SetCommandReply(unsigned long cmdId, redisReply *cmdReply)
{
    lock_guard<mutex> lock(m_lock);
    unsigned long tmpCmdId = 0;
    while (m_cmdQueue.size() > 0) {
        tmpCmdId = m_cmdQueue.front();
        m_cmdQueue.pop();
        //this command timeout
        auto it = m_cmdMap.find(tmpCmdId);
        shared_ptr<CCommand> tmpCmd = it->second;
        bool hasTimeOutCmds = false;
        if (tmpCmdId != cmdId) {
            if (it != m_cmdMap.end()) {
                tmpCmd->SetCommState(CCommandState::TIMEOUT);
                m_failedOrWaitList.push_back(tmpCmdId);
                hasTimeOutCmds = true;
            }
        }
        if (tmpCmdId != cmdId) {
            if (!hasTimeOutCmds) {
                if (cmdReply != nullptr) {
                    tmpCmd->SetPromiseValue(cmdReply);
                }
                else {
                    //In fact the hiredis keep the reply not null when call this function
                    HIREDIS_LOG_ERROR("Redis command get a null reply\n");
                    tmpCmd->SetPromiseValue(nullptr);
                }
            }
            else {
                //if has commands timeout,try again later,must keep the the order of commands
                //
                tmpCmd->SetCommState(CCommandState::TIMEOUT);
                m_failedOrWaitList.push_back(tmpCmdId);
            }
            break;
        }
    }

}

bool CRedisConnection::InitHiredis()
{
    m_pRedisContext->data = this;
    if (redisLibeventAttach(m_pRedisContext, m_pEventBase) != REDIS_OK) {
        SetConnState(enConnState::INIT_ERROR);
        HIREDIS_LOG_ERROR("redisLibeventAttach failed\n");
        return false;
    }
    if (redisAsyncSetConnectCallback(m_pRedisContext, &CRedisConnection::lcb_OnConnectCallback) != REDIS_OK) {
        SetConnState(enConnState::INIT_ERROR);
        HIREDIS_LOG_ERROR("redisAsyncSetConnectCallback failed\n");
        return false;
    }
    if (redisAsyncSetDisconnectCallback(m_pRedisContext, &CRedisConnection::lcb_OnDisconnectCallback) != REDIS_OK) {
        SetConnState(enConnState::INIT_ERROR);
        HIREDIS_LOG_ERROR("redisAsyncSetDisconnectCallback failed\n");
        return false;
    }
    redisContext *c = &(m_pRedisContext->c);
    m_closedEvent = event_new(m_pEventBase,
                              c->fd,
                              EV_CLOSED,
                              (event_callback_fn) &CRedisConnection::lcb_OnDisconnectEventCallback,
                              this);
    return true;
}

bool CRedisConnection::InitLibevent()
{
    m_pEventBase = event_base_new();
    if (nullptr == m_pEventBase) {
        HIREDIS_LOG_ERROR("Create event base error\n");
        return false;
    }
    return true;
}

bool CRedisConnection::RunEventLoop()
{
    ignore_pipe();
    event_base_dispatch(m_pEventBase);
    return true;
}

unsigned long CRedisConnection::GenCommandId()
{
    return ++m_cmdId;
}

//构建cmd
shared_ptr<CCommand> CRedisConnection::CreateCommand(const char *cmd, va_list vaList)
{
    return make_shared<CCommand>(GenCommandId(), cmd, vaList);
}

}
#endif //EXHIREDIS_REDIS_CONN_H
