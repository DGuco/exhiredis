//
// Created by dguco on 18-12-24.
//

#ifndef EXHIREDIS_REDIS_LOCK_HPP
#define EXHIREDIS_REDIS_LOCK_HPP
#include <cstring>
#include <memory>
#include "rlock.hpp"

using namespace std;
namespace exhiredis
{
class CRedisConnection;
class CRedisClients;
class CRedisLock: public IRlock
{
public:
    CRedisLock(const shared_ptr<CRedisClients> &redisClients,
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
    shared_ptr<CRedisClients> m_pRedisClients;
    string m_sLockName;
};
}
#endif //EXHIREDIS_REDIS_LOCK_HPP
