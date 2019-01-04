//
// Created by dguco on 19-1-3.
//

#include "redis_lock.h"
#include "exhiredis/rstl/rscript.h"
#include "exhiredis/comman_def.h"

namespace exhiredis
{
CRedisLock::CRedisLock(const shared_ptr<CRedisClients> &redisClients,
                       const string &m_sLockName)
    : m_pRedisClients(redisClients),
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
    shared_ptr<RScript> tmpScript = make_shared<RScript>(m_pRedisClients);
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
    tmpScript->EvalReturnInt(cmd,
                             {m_sLockName},
                             {to_string(leaseTime), to_string(threadId)},
                             eCommandModel::NOT_READ_ONLY);
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
    shared_ptr<RScript> tmpScript = make_shared<RScript>(m_pRedisClients);
//    tmpScript->EvalReturnInt(cmd, {m_sLockName}, {to_string(leaseTime), to_string(threadId)});
}
}