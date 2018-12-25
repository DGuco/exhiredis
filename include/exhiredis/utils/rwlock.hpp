//
// Created by dguco on 18-12-25.
// pthread rwlock
//

#ifndef EXHIREDIS_RWLOCK_HPP
#define EXHIREDIS_RWLOCK_HPP
#include <pthread.h>
#include <exception>
#include <system_error>
#include "exception.hpp"
#include "log.hpp"
namespace exhiredis
{
class CRWLock
{
public:

    CRWLock();
    ~CRWLock();
    CRWLock(const CRWLock &) = delete;
    CRWLock(const CRWLock &&) = delete;
    CRWLock &operator=(const CRWLock &) = delete;
    void ReadLock() const;
    void WriteLock() const;
    void TryReadLock() const;
    void TryWriteLock() const;
    void Unlock() const;
private:
    mutable pthread_rwlock_t m_sect;
};

CRWLock::CRWLock()
{
    int ret = pthread_rwlock_init(&m_sect, NULL);
    if (ret != 0) {
        throw CRException("pthread_rwlock_init error");
    }
}

CRWLock::~CRWLock()
{
    int ret = pthread_rwlock_destroy(&m_sect);
    if (ret != 0) {
        HIREDIS_LOG_ERROR("CRWLock ~CRWLock error: %s", string(strerror(ret)));;
    }
}

void CRWLock::ReadLock() const
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

void CRWLock::WriteLock() const
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

void CRWLock::TryReadLock() const
{
    int ret = pthread_rwlock_tryrdlock(&m_sect);
    if (ret != 0) {
        if (ret == EDEADLK) {
            throw CRException("[CRWLock::TryReadLock] pthread_rwlock_tryrdlock dead lock error", ret);
        }
        else {
            throw CRException("[CRWLock::TryReadLock] pthread_rwlock_tryrdlock error", ret);
        }
    }

}

void CRWLock::TryWriteLock() const
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

void CRWLock::Unlock() const
{
    int ret = pthread_rwlock_unlock(&m_sect);
    if (ret != 0) {
        throw CRException(" pthread_rwlock_unlock error", ret);
    }

}
}
#endif //EXHIREDIS_RWLOCK_HPP
