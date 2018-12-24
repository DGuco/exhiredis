//
// Created by dguco on 18-12-24.
//

#ifndef EXHIREDIS_REDIS_LOCK_HPP
#define EXHIREDIS_REDIS_LOCK_HPP
#include <cstring>
#include "exhiredis/redis_connection.hpp"
#include "exhiredis/rstl/rscript.hpp"
#include "rlock.hpp"

namespace exhiredis
{
class CRedisLock: public IRlock
{
public:
    CRedisLock(const shared_ptr<CRedisConnection> &m_pRedisConn,
               const string &m_sLockName);
    void Lock() override;
    bool TryLock() override;
    /**
     * @param time ms
     * @return
     */
    bool TryLock(long time) override;
    void Unlock() override;

private:
    void TryLockAsync(long leaseTime, long threadId);
    void UnLockAsync(long threadId);
private:
    shared_ptr<CRedisConnection> m_pRedisConn;
    string m_sLockName;
};

CRedisLock::CRedisLock(const shared_ptr<CRedisConnection> &m_pRedisConn,
                       const string &m_sLockName)
    : m_pRedisConn(m_pRedisConn),
      m_sLockName(m_sLockName)
{

}

void CRedisLock::Lock()
{
    TryLock(-1);
}

bool CRedisLock::TryLock()
{
    TryLock(-1);
}

bool CRedisLock::TryLock(long time)
{
    TryLock(time);
}

void CRedisLock::Unlock()
{

}

void CRedisLock::TryLockAsync(long leaseTime, long threadId)
{
    shared_ptr<RScript> tmpScript = make_shared<RScript>(m_pRedisConn);
    string cmd = "if (redis.call('exists', KEYS[1]) == 0) then " \
        "redis.call('hset', KEYS[1], ARGV[2], 1); " \
        "redis.call('pexpire', KEYS[1], ARGV[1]); " \
        "return nil; " \
        "end; " \
        "if (redis.call('hexists', KEYS[1], ARGV[2]) == 1) then " \
        "redis.call('hincrby', KEYS[1], ARGV[2], 1); " \
        "redis.call('pexpire', KEYS[1], ARGV[1]); " \
        "return nil; " \
        "end; " \
        "return redis.call('pttl', KEYS[1]);";
    tmpScript->EvalReturnInt(cmd, {m_sLockName}, {to_string(leaseTime), to_string(threadId)});
}

void CRedisLock::UnLockAsync(long threadId)
{
    string cmd = "if (redis.call('exists', KEYS[1]) == 0) then " \
        "redis.call('publish', KEYS[2], ARGV[1]); " \
        "return 1; " \
        "end;" \
        "if (redis.call('hexists', KEYS[1], ARGV[3]) == 0) then " \
        "return nil;" \
        "end; " \
        "local counter = redis.call('hincrby', KEYS[1], ARGV[3], -1); " \
        "if (counter > 0) then " \
        "redis.call('pexpire', KEYS[1], ARGV[2]); " \
        "return 0; " \
        "else " \
        "redis.call('del', KEYS[1]); " \
        "redis.call('publish', KEYS[2], ARGV[1]); " \
        "return 1; "\
        "end; " \
        "return nil;";
    shared_ptr<RScript> tmpScript = make_shared<RScript>(m_pRedisConn);
//    tmpScript->EvalReturnInt(cmd, {m_sLockName}, {to_string(leaseTime), to_string(threadId)});
}
}
#endif //EXHIREDIS_REDIS_LOCK_HPP
