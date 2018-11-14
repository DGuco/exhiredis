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
    static const int NO_REPLY = -1;   // No reply yet
    static const int OK_REPLY = 0;    // Successful reply of the expected type
    static const int NIL_REPLY = 1;   // Got a nil reply
    static const int ERROR_REPLY = 2; // Got an error reply
    static const int SEND_ERROR = 3;  // Could not send to server
    static const int WRONG_TYPE = 4;  // Got reply, but it was not the expected type
    static const int TIMEOUT = 5;     // No reply, timed out
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
    void SetCommState(const int iCommState);
private:
    const unsigned long m_iCommandId;  //cmd id
    atomic_int m_iCommState;
    shared_ptr<promise<redisReply *>> m_pPromise;
    redisReply *m_pReply;
    const string m_sCmd;
    va_list m_param;
};

CCommand::CCommand(unsigned long id, const char *cmd, va_list vaList)
    : m_iCommandId(id),
      m_pPromise(make_shared<promise<redisReply *>>(promise<redisReply *>())),
      m_pReply(nullptr),
      m_sCmd(cmd)
{
    m_iCommState.store(CCommandState::NO_REPLY);
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
    return m_iCommState.load();
}

void CCommand::SetCommState(const int iCommState)
{
    m_iCommState.store(iCommState);
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
