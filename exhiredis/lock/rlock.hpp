//
// Created by dguco on 18-10-18.
// redis Lock interface
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
    virtual void Lock() = 0;
    virtual bool TryLock() = 0;
    virtual bool TryLock(long time) = 0;
    virtual void Unlock() = 0;
};
}
#endif //EXHIREDIS_RLOCK_HPP
