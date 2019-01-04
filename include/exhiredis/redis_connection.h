//
// Created by dguco on 18-10-17.
// redis connection
//

#ifndef EXHIREDIS_REDIS_CONN_H
#define EXHIREDIS_REDIS_CONN_H
#include <hiredis/adapters/libevent.h>
#include <hiredis/async.h>
#include <hiredis/hiredis.h>
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
#include <ctime>
#include <ratio>
#include <atomic>
#include "exhiredis/utils/exception.h"
#include "exhiredis/utils/signal.h"
#include "exhiredis/utils/log.h"
#include "exhiredis/utils/uuid.h"
#include "exhiredis/robject/rint.h"
#include "comman_def.h"

using std::chrono::system_clock;
namespace exhiredis
{
class CCmdParam;
class CCommand;
class CRedisConnection
{
public:
    //构造函数
    CRedisConnection();
    CRedisConnection(const CRedisConnection &tmp) = delete;
    CRedisConnection(CRedisConnection &&tmp) = delete;
    CRedisConnection &operator=(const CRedisConnection &tmp) = delete;
    //析构函数
    virtual ~CRedisConnection();
    //connect redis
    bool Connect(const string &host, int portn, bool isReconn = false);
    //reconnect to redis
    bool ReConnect();
    //connect redis
    bool ConnectToUnix(const string &address);
    //get status of the connecton
    enConnState GetConnState();
    //set status of the connecton
    void SetConnState(enConnState eConnState);
    //create cmd
    shared_ptr<CCommand> CreateCommand(const char *format, vector<shared_ptr<CCmdParam>> &param);
    //execute redis
    shared_ptr<CCommand> RedisvAsyncCommand(const char *format, vector<shared_ptr<CCmdParam>> &param);
private:
    //execute redis
    int SendCommandAsync(shared_ptr<CCommand> &command);
    //create command id
    unsigned long GenCommandId();
    //shutdown the connection
    void ShutDown();
    //flush all command
    void RetryFailedOrWaitCommands();
    //init hiredis
    bool InitHiredis();
    //init libevent
    bool InitLibevent();
    //run eventloop
    bool RunEventLoop();
    //set command reply
    void SetCommandReply(unsigned long cmdId, redisReply *cmdReply);
    //start command check timer
    void StartCheckTimer();
    //check timerOut command
    void CheckTimeOutCommand();
private:
    //connected callback
    static void lcb_OnConnectCallback(const redisAsyncContext *context, int status);
    //disconnected callback(this callback will not call until we found the connection is gone when we send failed or we closed ourselves)
    static void lcb_OnDisconnectCallback(const redisAsyncContext *context, int status);
    //the server closed our connection
    static void lcb_OnDisconnectEventCallback(evutil_socket_t fd, short event, void *data);
    //redis command callback
    static void lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata);
    //check timeout commands
    static void lcb_OnTimeoutCallback(evutil_socket_t fd, short events, void *arg);
private:
    redisAsyncContext *m_pRedisContext;
    event_base *m_pEventBase;
    event *m_pClosedEvent;
    event *m_pTimeOutEvent;
    shared_ptr<thread> m_pEventLoopThread;
    string m_sHost;
    int m_iPort;
    string m_sAddress;
    enConnState m_connState;
    mutex m_lock;
    atomic_ulong m_cmdId;
    unordered_map<unsigned long, shared_ptr<CCommand>> m_cmdMap;
    list<unsigned long> m_cmdList;
    list<unsigned long> m_failedOrWaitList;//按照send的顺序排列
};
}
#endif //EXHIREDIS_REDIS_CONN_H
