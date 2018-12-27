//
// Created by dguco on 18-10-24.
// redis cmd
//

#ifndef EXHIREDIS_COMMAND_H
#define EXHIREDIS_COMMAND_H

#include <future>
#include "robject/robject.hpp"

namespace exhiredis
{
enum class eCommandState
{
    NOT_SEND = 1,  //Has not send to server
    SEND_ERROR = 2,  // Could not send to server
    NO_REPLY_YET = 3,   // No reply yet
    TIMEOUT = 4,     // No reply, timed out
    REPLY_DONE = 5,     // redis reply done
    COMMAND_RETRYING = 6,     // command retrying
};
class CCommand
{
public:
    //CCommand
    CCommand(unsigned long id, const char *cmd, va_list vaList);
    //~CCommand
    ~CCommand();
    //get cmd id
    const unsigned long GetCommandId() const;
    //set promise value
    void SetPromiseValue(redisReply *reply);
    //set promise exception
    void SetPromiseException(exception_ptr exception);
    //get cmd reply promise
    shared_ptr<promise<redisReply *>> &GetPromise();
    //get cmd status
    eCommandState GetCommState() const;
    //set cmd status
    void SetCommState(eCommandState iCommState);
    //get cmd
    const char *GetCmd();
    //to string
    const char *ToString();
private:
    const unsigned long m_iCommandId;  //cmd id
    eCommandState m_iCommState;
    shared_ptr<promise<redisReply *>> m_pPromise;
    redisReply *m_pReply;
    const char *m_sCmd;
public:
    va_list m_param;
};

CCommand::CCommand(unsigned long id, const char *cmd, va_list vaList)
    : m_iCommandId(id),
      m_pPromise(make_shared<promise<redisReply *>>(promise<redisReply *>())),
      m_pReply(nullptr),
      m_sCmd(cmd),
      m_iCommState(eCommandState::NOT_SEND)
{
    va_copy(m_param, vaList);
}

CCommand::~CCommand()
{
    if (m_pReply != nullptr) {
        //释放reply object
        freeReplyObject(m_pReply);
    }
    va_end(m_param);
}

const unsigned long CCommand::GetCommandId() const
{
    return m_iCommandId;
}

eCommandState CCommand::GetCommState() const
{
    return m_iCommState;
}

void CCommand::SetCommState(eCommandState iCommState)
{
    m_iCommState = iCommState;
}

void CCommand::SetPromiseValue(redisReply *reply)
{
    m_pPromise->set_value(reply);
    this->m_pReply = reply;
}

void CCommand::SetPromiseException(exception_ptr exception)
{
    m_pPromise->set_exception(exception);
}

shared_ptr<promise<redisReply *>> &CCommand::GetPromise()
{
    return m_pPromise;
}

const char *CCommand::GetCmd()
{
    return m_sCmd;
}

const char *CCommand::ToString()
{
    return m_sCmd;
}
}
#endif //EXHIREDIS_COMMAND_H
