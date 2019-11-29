//
// Created by dguco on 18-10-17.
// redis connection
//

#ifndef EXHIREDIS_REDIS_CONN_H
#define EXHIREDIS_REDIS_CONN_H
#include <string>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <event.h>
#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <ratio>
#include <atomic>
#include "utils/exception.h"
#include "utils/signal.h"
#include "utils/log.h"
#include "comman_def.h"
#include "../hiredis/adapters/libevent.h"
#include "../hiredis/async.h"
#include "../hiredis/hiredis.h"

namespace exhiredis
{
class CCmdParam;
class CRedisAsyncConnection
{
public:
    //构造函数
    CRedisAsyncConnection();
    CRedisAsyncConnection(const CRedisAsyncConnection &tmp) = delete;
    CRedisAsyncConnection(CRedisAsyncConnection &&tmp) = delete;
    CRedisAsyncConnection &operator=(const CRedisAsyncConnection &tmp) = delete;
    //析构函数
    virtual ~CRedisAsyncConnection();
    //connect redis
    bool Connect(const string &host, int portn, bool isReconn = false);
    //reconnect to redis
    bool ReConnect();
    //connect redis
    bool ConnectToUnix(const string &address);
    //execute redis
    void SendCommandAsync(const vector<std::string> &commands,redisCallbackFn *fn);
    enConnState GetConnState();
private:
    void SetConnState(enConnState m_connState);
    //shutdown the connection
    void ShutDown();
    //init hiredis
    bool InitHiredis();
    //init libevent
    void InitLibevent();
    //run eventloop
    bool RunEventLoop();
private:
    //connected callback
    static void lcb_OnConnectCallback(const redisAsyncContext *context, int status);
    //disconnected callback(this callback will not call until we found the connection is gone when we send failed or we closed ourselves)
    static void lcb_OnDisconnectCallback(const redisAsyncContext *context, int status);
    //redis command callback
    static void lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata);
    //check timeout commands
    static void lcb_OnTimeoutCallback(evutil_socket_t fd, short events, void *arg);
private:
    redisAsyncContext *m_pRedisContext;
    event_base *m_pEventBase;
    thread m_eventLoopThread;
    string m_sHost;
    int m_iPort;
    string m_sAddress;
    enConnState  m_connState;
};
}
#endif //EXHIREDIS_REDIS_CONN_H
