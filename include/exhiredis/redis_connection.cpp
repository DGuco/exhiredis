//
// Created by dguco on 19-1-4.
//
#include "redis_connection.h"
#include "command.h"
#include "command_param.hpp"

namespace exhiredis
{

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
    StartCheckTimer();
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

bool CRedisConnection::ConnectToUnix(const string &address)
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

shared_ptr<CCommand> CRedisConnection::RedisvAsyncCommand(const char *format,
                                                          vector<shared_ptr<CCmdParam>> &param)
{
    shared_ptr<CCommand> tmpCommand = CreateCommand(format, param);
    lock_guard<mutex> lk(m_lock);
    unsigned long commandId = tmpCommand->GetCommandId();
    m_cmdMap.insert(make_pair(commandId, tmpCommand));
    m_cmdList.push_back(commandId);
    if (m_connState == enConnState::CONNECTED) {
        //retry failed or commands
        RetryFailedOrWaitCommands();
        if (SendCommandAsync(tmpCommand) != REDIS_OK) {
            tmpCommand->SetCommState(eCommandState::SEND_ERROR);
            m_failedOrWaitList.push_back(commandId); //失败后再尝试一次
            HIREDIS_LOG_ERROR("Send redis command failed,cmd = %s", tmpCommand->ToString());
        }
        else {
            tmpCommand->SetCommState(eCommandState::NO_REPLY_YET);
        }
    }
    else {
        m_failedOrWaitList.push_back(commandId);
    }
    return std::move(tmpCommand);
}

int CRedisConnection::SendCommandAsync(shared_ptr<CCommand> &command)
{
    int status = redisAsyncFormattedCommand(m_pRedisContext,
                                            &CRedisConnection::lcb_OnCommandCallback,
                                            (void *) command->GetCommandId(),
                                            command->GetCmd(),
                                            (size_t) command->GetCmdLen());
    command->SetSendTime(system_clock::to_time_t(system_clock::now()));
    return status;
}

void CRedisConnection::ShutDown()
{
    if (m_pClosedEvent != nullptr) {
        event_free(m_pClosedEvent);
        m_pClosedEvent = nullptr;
    }

    if (m_pTimeOutEvent != nullptr) {
        event_free(m_pTimeOutEvent);
        m_pTimeOutEvent = nullptr;
    }

    if (m_pRedisContext != nullptr) {
        redisAsyncFree(m_pRedisContext);
        m_pRedisContext = nullptr;
    }
    if (m_pEventBase != nullptr) {
        event_base_loopbreak(m_pEventBase);
        event_base_free(m_pEventBase);
        m_pEventBase = nullptr;
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
                eCommandState state = command->GetCommState();
                if (status == REDIS_OK) {
                    command->SetCommState(eCommandState::COMMAND_RETRYING);
                }
                else {
                    if (state == eCommandState::SEND_ERROR || state == eCommandState::REPLY_FAILED) {
                        HIREDIS_LOG_ERROR("Retry command failed,command = %s", command->ToString());
                    }
                }
            }
            tmpList.pop_front();
        }
    }
}

void CRedisConnection::SetCommandReply(unsigned long cmdId, redisReply *cmdReply)
{
    lock_guard<mutex> lock(m_lock);
    unsigned long tmpCmdId = 0;
    auto cmdIdIt = m_cmdList.begin();
    bool hasTimeOutCmds = false;
    while (cmdIdIt != m_cmdList.end()) {
        tmpCmdId = *cmdIdIt;
        auto cmdIt = m_cmdMap.find(tmpCmdId);
        if (cmdIt == m_cmdMap.end()) {
            cmdIdIt++;
            continue;
        }
        shared_ptr<CCommand> tmpCmd = cmdIt->second;
        //redis单线程处理command如果收到的reply前面有没有收到reply的command则这个command reply failed
        if (tmpCmdId != cmdId) {
            tmpCmd->SetCommState(eCommandState::REPLY_FAILED);
            m_failedOrWaitList.push_back(tmpCmdId);
            hasTimeOutCmds = true;
        }
        else {
            if (!hasTimeOutCmds) {
                if (cmdReply != nullptr) {
                    tmpCmd->SetPromiseValue(cmdReply);
                }
                else {
                    //In fact the hiredis keep the reply not null when call this function
                    HIREDIS_LOG_ERROR("Redis command get a null reply\n");
                    tmpCmd->SetPromiseValue(nullptr);
                }
                //receive reply,remove command
                //删除commandd
                m_cmdMap.erase(tmpCmdId);
                m_cmdList.erase(cmdIdIt);
            }
            else {
                /*if has commands timeout,try again later,must keep the the order of commands
                 *redis单线程处理command如果收到的reply前面有没有收到reply的command则这个command已经超时
                 *为了保证redis command的数据正确性必须保证command的执行顺序和发生顺序是一直的，
                 *所以把该命令加入失败队列重新发送
                **/
                tmpCmd->SetCommState(eCommandState::REPLY_FAILED);
                m_failedOrWaitList.push_back(tmpCmdId);
            }
            break;
        }
        cmdIdIt++;
    }
    if (hasTimeOutCmds) {
        RetryFailedOrWaitCommands();
    }
}

void CRedisConnection::StartCheckTimer()
{
    if (m_pTimeOutEvent == nullptr) {
        m_pTimeOutEvent = new event;
    }
    //todo check timeount config
    struct timeval tv;
    tv.tv_sec = 5;    // 秒
    tv.tv_usec = 0;  // 微秒
    //timer 事件
    evtimer_assign(m_pTimeOutEvent, m_pEventBase, &CRedisConnection::lcb_OnTimeoutCallback, (void *) this);
    evtimer_add(m_pTimeOutEvent, &tv);
}

void CRedisConnection::CheckTimeOutCommand()
{
    time_t time = system_clock::to_time_t(system_clock::now());
    lock_guard<mutex> lock(m_lock);
    auto it = m_cmdList.begin();
    while (it != m_cmdList.end()) {
        auto cmdIt = m_cmdMap.find(*it);
        if (cmdIt != m_cmdMap.end()) {
            shared_ptr<CCommand> cmd = cmdIt->second;
            if (time - cmd->GetSendTime() >= 5 && cmd->GetCommState() != eCommandState::NOT_SEND) {
                //command timeout return null or throw an exception ?
                //返回null或者抛出异常
                cmd->SetPromiseValue(nullptr);
                it = m_cmdList.erase(it);
                m_cmdMap.erase(cmd->GetCommandId());
            }
            else {
                break;
            }
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
    m_pClosedEvent = event_new(m_pEventBase,
                               c->fd,
                               EV_CLOSED,
                               &CRedisConnection::lcb_OnDisconnectEventCallback,
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
shared_ptr<CCommand> CRedisConnection::CreateCommand(const char *format, vector<shared_ptr<CCmdParam>> &param)
{
    unsigned long cmdId = GenCommandId();
    char *cmd;
    int len;
    len = CCommand::RedisvFormatCommand(&cmd, format, param);

    if (len < 0)
        throw CRException("CreateCommand failed,error type : " + len);
    return std::move(make_shared<CCommand>(cmdId, cmd, len));
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
        conn->RetryFailedOrWaitCommands();
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

void CRedisConnection::lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata)
{
    CRedisConnection *conn = (CRedisConnection *) context->data;
    unsigned long cmdId = (unsigned long) privdata;
    redisReply *cmdReply = (redisReply *) reply;
    conn->SetCommandReply(cmdId, cmdReply);
}

void CRedisConnection::lcb_OnDisconnectEventCallback(evutil_socket_t fd, short event, void *data)
{
    CRedisConnection *conn = (CRedisConnection *) data;
    if (conn != nullptr) {
        conn->SetConnState(enConnState::DISCONNECTED);
        HIREDIS_LOG_ERROR("Disconnected from redis");
        conn->ReConnect();
    }
}

void CRedisConnection::lcb_OnTimeoutCallback(evutil_socket_t fd, short events, void *arg)
{
    CRedisConnection *conn = (CRedisConnection *) arg;
    if (conn != nullptr) {
        conn->CheckTimeOutCommand();
        conn->StartCheckTimer();
    }
}
}