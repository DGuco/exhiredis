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
class CCommandState
{
public:
    static const int NOT_SEND = 1;  //Has not send to server
    static const int SEND_ERROR = 2;  // Could not send to server
    static const int NO_REPLY = 3;   // No reply yet
    static const int TIMEOUT = 4;     // No reply, timed out
    static const int REPLY_DONE = 5;     // redis reply done
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
    const int GetCommState() const;
    //set cmd status
    void SetCommState(unsigned short iCommState);
private:
    const unsigned long m_iCommandId;  //cmd id
    unsigned short m_iCommState;
    shared_ptr<promise<redisReply *>> m_pPromise;
    redisReply *m_pReply;
    const char *m_sCmd;
    va_list m_param;
};

CCommand::CCommand(unsigned long id, const char *cmd, va_list vaList)
    : m_iCommandId(id),
      m_pPromise(make_shared<promise<redisReply *>>(promise<redisReply *>())),
      m_pReply(nullptr),
      m_sCmd(cmd),
      m_iCommState(CCommandState::NOT_SEND)
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

const int CCommand::GetCommState() const
{
    return m_iCommState;
}

void CCommand::SetCommState(unsigned short iCommState)
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
}
#endif //EXHIREDIS_COMMAND_H
