//
// Created by dguco on 18-10-24.
// redis cmd
//

#ifndef EXHIREDIS_COMMAND_H
#define EXHIREDIS_COMMAND_H

#include <future>
#include <cstring>
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
    CCommand(unsigned long id, const char *cmd, const vector<shared_ptr<CCmdParam>> &m_paramList);
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
    const vector<shared_ptr<CCmdParam>> &GetParamList() const;
private:
    const unsigned long m_iCommandId;  //cmd id
    eCommandState m_iCommState;
    shared_ptr<promise<redisReply *>> m_pPromise;
    redisReply *m_pReply;
    const char *m_sCmd;
    const vector<shared_ptr<CCmdParam>> m_paramList;
};

CCommand::CCommand(unsigned long id, const char *cmd, const vector<shared_ptr<CCmdParam>> &m_paramList)
    : m_iCommandId(id),
      m_pPromise(make_shared<promise<redisReply *>>(promise<redisReply *>())),
      m_pReply(nullptr),
      m_sCmd(cmd),
      m_iCommState(eCommandState::NOT_SEND),
      m_paramList(std::move(m_paramList))
{
}

CCommand::~CCommand()
{
    if (m_pReply != nullptr) {
        //释放reply object
        freeReplyObject(m_pReply);
    }
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

const vector<shared_ptr<CCmdParam>> &CCommand::GetParamList() const
{
    return m_paramList;
}
}
#endif //EXHIREDIS_COMMAND_H
