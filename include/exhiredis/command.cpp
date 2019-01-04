//
// Created by dguco on 19-1-3.
//

#include <memory>
#include <future>
#include <vector>
#include "command.h"
#include "utils/sds.h"
#include "command_param.hpp"

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

uint32_t CCommand::CountDigits(uint64_t v)
{
    uint32_t result = 1;
    for (;;) {
        if (v < 10) return result;
        if (v < 100) return result + 1;
        if (v < 1000) return result + 2;
        if (v < 10000) return result + 3;
        v /= 10000U;
        result += 4;
    }
}

/* Helper that calculates the bulk length given a certain string length. */
size_t CCommand::Bulklen(size_t len)
{
    return 1 + CountDigits(len) + 2 + len + 2;
}

time_t CCommand::GetSendTime() const
{
    return m_tSendTime;
}

void CCommand::SetSendTime(time_t sendTime)
{
    sendTime = sendTime;
}

int CCommand::RedisvFormatCommand(char **target, const char *format, vector<shared_ptr<CCmdParam>> &param)
{
    const char *c = format;
    char *cmd = NULL; /* final command */
    int pos; /* position in final command */
    sds curarg, newarg; /* current argument */
    int touched = 0; /* was the current argument touched? */
    char **curargv = NULL, **newargv = NULL;
    int argc = 0;
    int totlen = 0;
    int error_type = 0; /* 0 = no error; -1 = memory error; -2 = format error */
    int j;

    /* Abort if there is not target to set */
    if (target == NULL)
        return -1;

    /* Build the command string accordingly to protocol */
    curarg = sdsempty();
    if (curarg == NULL)
        return -1;

    size_t paraIndex = 0;
    while (*c != '\0') {
        if (*c != '%' || c[1] == '\0') {
            if (*c == ' ') {
                if (touched) {
                    newargv = (char **) realloc(curargv, sizeof(char *) * (argc + 1));
                    if (newargv == NULL) goto memory_err;
                    curargv = newargv;
                    curargv[argc++] = curarg;
                    totlen += Bulklen(sdslen(curarg));

                    /* curarg is put in argv so it can be overwritten. */
                    curarg = sdsempty();
                    if (curarg == NULL) goto memory_err;
                    touched = 0;
                }
            }
            else {
                newarg = sdscatlen(curarg, c, 1);
                if (newarg == NULL) goto memory_err;
                curarg = newarg;
                touched = 1;
            }
        }
        else {
            char *arg;
            size_t size;

            /* Set newarg so it can be checked even if it is not touched. */
            newarg = curarg;
            if (paraIndex >= param.size()) {
                goto format_err;
            }
            switch (c[1]) {
                case 's':arg = param.at(paraIndex++)->GetChar();
                    size = strlen(arg);
                    if (size > 0)
                        newarg = sdscatlen(curarg, arg, size);
                    break;
                case 'b':arg = param.at(paraIndex++)->GetChar();
                    size = param.at(paraIndex++)->GetSize();
                    if (size > 0)
                        newarg = sdscatlen(curarg, arg, size);
                    break;
                default: {
                    goto format_err;
                }
            }
            if (newarg == NULL) goto memory_err;
            curarg = newarg;

            touched = 1;
            c++;
        }
        c++;
    }

    /* Add the last argument if needed */
    if (touched) {
        newargv = (char **) realloc(curargv, sizeof(char *) * (argc + 1));
        if (newargv == NULL) goto memory_err;
        curargv = newargv;
        curargv[argc++] = curarg;
        totlen += Bulklen(sdslen(curarg));
    }
    else {
        sdsfree(curarg);
    }

    /* Clear curarg because it was put in curargv or was free'd. */
    curarg = NULL;

    /* Add bytes needed to hold multi bulk count */
    totlen += 1 + CountDigits(argc) + 2;

    /* Build the command at protocol level */
    cmd = (char *) malloc(totlen + 1);
    if (cmd == NULL) goto memory_err;

    pos = sprintf(cmd, "*%d\r\n", argc);
    for (j = 0; j < argc; j++) {
        pos += sprintf(cmd + pos, "$%zu\r\n", sdslen(curargv[j]));
        memcpy(cmd + pos, curargv[j], sdslen(curargv[j]));
        pos += sdslen(curargv[j]);
        sdsfree(curargv[j]);
        cmd[pos++] = '\r';
        cmd[pos++] = '\n';
    }
    assert(pos == totlen);
    cmd[pos] = '\0';

    free(curargv);
    *target = cmd;
    return totlen;

    format_err:
    error_type = -2;
    goto cleanup;

    memory_err:
    error_type = -1;
    goto cleanup;

    cleanup:
    if (curargv) {
        while (argc--)
            sdsfree(curargv[argc]);
        free(curargv);
    }

    sdsfree(curarg);
    free(cmd);

    return error_type;
}
}