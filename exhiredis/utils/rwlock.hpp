//
// Created by dguco on 18-12-25.
// pthread rwlock
//

#ifndef EXHIREDIS_RWLOCK_HPP
#define EXHIREDIS_RWLOCK_HPP
#include <pthread.h>
#include <exception>
#include <system_error>
#include "exception.h"
#include "log.h"
namespace exhiredis
{
class CRWLock
{
public:
   CRWLock()
    {
        int ret = pthread_rwlock_init(&m_sect, NULL);
        if (ret != 0) {
            throw CRException("pthread_rwlock_init error");
        }
    }

    ~CRWLock()
    {
        int ret = pthread_rwlock_destroy(&m_sect);
        if (ret != 0) {
            HIREDIS_LOG_ERROR("CRWLock ~CRWLock error: %s", string(strerror(ret)));;
        }
    }

    void ReadLock() const
    {
        int ret = pthread_rwlock_rdlock(&m_sect);
        if (ret != 0) {
            if (ret == EDEADLK) {
                throw CRException("pthread_rwlock_rdlock dead lock error", ret);
            }
            else {
                throw CRException("pthread_rwlock_rdlock error", ret);
            }
        }
    }

    void WriteLock() const
    {
        int ret = pthread_rwlock_wrlock(&m_sect);
        if (ret != 0) {
            if (ret == EDEADLK) {
                throw CRException("pthread_rwlock_wrlock dead lock error", ret);
            }
            else {
                throw CRException("pthread_rwlock_wrlock error", ret);
            }
        }

    }

    void TryReadLock() const
    {
        int ret = pthread_rwlock_tryrdlock(&m_sect);
        if (ret != 0) {
            if (ret == EDEADLK) {
                throw CRException("[TryReadLock] pthread_rwlock_tryrdlock dead lock error", ret);
            }
            else {
                throw CRException("[TryReadLock] pthread_rwlock_tryrdlock error", ret);
            }
        }

    }

    void TryWriteLock() const
    {
        int ret = pthread_rwlock_trywrlock(&m_sect);
        if (ret != 0) {
            if (ret == EDEADLK) {
                throw CRException("pthread_rwlock_trywrlock dead lock error", ret);
            }
            else {
                throw CRException("pthread_rwlock_trywrlock error", ret);
            }
        }

    }

    void Unlock() const
    {
        int ret = pthread_rwlock_unlock(&m_sect);
        if (ret != 0) {
            throw CRException(" pthread_rwlock_unlock error", ret);
        }

    }
private:
    mutable pthread_rwlock_t m_sect;
};
}
#endif //EXHIREDIS_RWLOCK_HPP
