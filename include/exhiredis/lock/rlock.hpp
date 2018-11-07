//
// Created by dguco on 18-10-18.
// redis lock interface
//

#ifndef EXHIREDIS_RLOCK_HPP
#define EXHIREDIS_RLOCK_HPP
#include <iostream>
#include <chrono>
namespace exhiredis
{
class IRlock
{
public:
    virtual void lock() = 0;
    virtual void lockInterruptibly() = 0;
    virtual bool tryLock() = 0;
    virtual bool tryLock(long time) = 0;
    virtual void unlock() = 0;
};
}
#endif //EXHIREDIS_RLOCK_HPP
