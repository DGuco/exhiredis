//
// Created by dguco on 19-1-3.
//

#include <memory>
#include <future>
#include <vector>
#include "command.h"
#include "utils/sds.h"

namespace exhiredis
{
    CCommand::CCommand(unsigned long id, const char *cmd, int cmdLen)
            : m_iCommandId(id),
              m_pPromise(make_shared<promise<redisReply * >>(promise<redisReply *>())),
              m_pReply(nullptr),
              m_sCmd(cmd),
              m_cmdLen(cmdLen),
              m_tSendTime(0),
              m_iCommState(eCommandState::NOT_SEND)
    {
    }

    CCommand::~CCommand()
    {
        if (m_pReply != nullptr) {
            //释放reply object
            freeReplyObject(m_pReply);
        }
        if (m_sCmd != nullptr) {
            free((void *) m_sCmd);
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

    const int CCommand::GetCmdLen() const
    {
        return m_cmdLen;
    }

    time_t CCommand::GetSendTime() const
    {
        return m_tSendTime;
    }

    void CCommand::SetSendTime(time_t sendTime)
    {
        sendTime = sendTime;
    }
}