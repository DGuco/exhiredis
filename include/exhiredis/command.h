//
// Created by dguco on 18-10-24.
// redis cmd
//

#ifndef EXHIREDIS_COMMAND_H
#define EXHIREDIS_COMMAND_H

#include <future>
#include <cstring>
#include <cassert>
#include <chrono>
#include <vector>
#include <hiredis/async.h>
#include "comman_def.h"

using namespace std;
using std::chrono::time_point;

namespace exhiredis
{
class CCmdParam;
class CCommand
{
public:
    //CCommand
    CCommand(unsigned long id, const char *cmd, int cmdLen);
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
    const int GetCmdLen() const;
    time_t GetSendTime() const;
    void SetSendTime(time_t sendTime);
public:
    /**
     * format command (copy from hiredis)
     * @param target
     * @param format
     * @return
     */
    static int RedisvFormatCommand(char **target, const char *format, vector<shared_ptr<CCmdParam>> &param);
    /**
     * (copy from hiredis)
     * @param v
     * @return
     */
private:
    static uint32_t CountDigits(uint64_t v);
    /**
     * (copy from hiredis)
     * @param len
     * @return
     */
    static size_t Bulklen(size_t len);
private:
    const unsigned long m_iCommandId;  //cmd id
    shared_ptr<promise<redisReply *>> m_pPromise;
    redisReply *m_pReply;
    const char *m_sCmd;
    const int m_cmdLen;
    time_t m_tSendTime;
    eCommandState m_iCommState;
};
}
#endif //EXHIREDIS_COMMAND_H
