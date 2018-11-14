//
// Created by dguco on 18-10-20.
//

#ifndef EXHIREDIS_SINGLETON_HPP
#define EXHIREDIS_SINGLETON_HPP

#include <memory>
#include <mutex>
using namespace std;

namespace exhiredis
{
mutex g_SingleTonLock;

template<typename T>
class CSingleton
{
public:
    CSingleton()
    {};
private:
    CSingleton(const CSingleton &temp) = delete;
    CSingleton &operator=(const CSingleton &temp) = delete;
private:
    static shared_ptr<T> spSingleton;
public:
    static shared_ptr<T> &GetSingletonPtr()
    {
        return spSingleton;
    }

    static shared_ptr<T> &CreateInstance()
    {
        lock_guard<mutex> lock(g_SingleTonLock);
        if (spSingleton == NULL) {
            spSingleton = make_shared<T>();
        }
        return spSingleton;
    }
};

}
#endif //EXHIREDIS_SINGLETON_HPP
