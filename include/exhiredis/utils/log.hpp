//
// Created by dguco on 18-10-20.
// exhiredis log
//

#ifndef EXHIREDIS_LOG_HPP
#define EXHIREDIS_LOG_HPP

#include <mutex>
#include "singleton.hpp"

namespace exhiredis
{
class CLog: public CSingleton<CLog>
{
public:
    enum class eLogLevel
    {
        TRACE = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };
public:
    template<typename... Args>
    int LogTrace(const char *vFmt, const Args &... args);
    template<typename... Args>
    int LogDebug(const char *vFmt, const Args &... args);
    template<typename... Args>
    int LogInfo(const char *vFmt, const Args &... args);
    template<typename... Args>
    int LogWarn(const char *vFmt, const Args &... args);
    template<typename... Args>
    int LogError(const char *vFmt, const Args &... args);
    template<typename... Args>
    int LogCritical(const char *vFmt, const Args &... args);
public:
    template<typename... Args>
    int Log(eLogLevel vPriority, const char *vFmt, const Args &... args);
private:
    std::mutex m_logLock;
};

template<> shared_ptr<CLog> CSingleton<CLog>::spSingleton = NULL;

template<typename... Args>
int CLog::LogTrace(const char *vFmt, const Args &... args)
{
    char buff[1024];
    sprintf(buff, vFmt, args...);
    {
        std::lock_guard<mutex> lock(m_logLock);
        printf("[HIREDIS] TRACE : %s\n", buff);
    }
    return 0;
}

template<typename... Args>
int CLog::LogDebug(const char *vFmt, const Args &... args)
{
    char buff[1024];
    sprintf(buff, vFmt, args...);
    {
        std::lock_guard<mutex> lock(m_logLock);
        printf("[HIREDIS] DEBUG : %s\n", buff);
    }
    return 0;
}
template<typename... Args>
int CLog::LogInfo(const char *vFmt, const Args &... args)
{
    char buff[1024];
    sprintf(buff, vFmt, args...);
    {
        std::lock_guard<mutex> lock(m_logLock);
        printf("[HIREDIS] INFO : %s\n", buff);
    }
    return 0;
}
template<typename... Args>
int CLog::LogWarn(const char *vFmt, const Args &... args)
{
    char buff[1024];
    sprintf(buff, vFmt, args...);
    {
        std::lock_guard<mutex> lock(m_logLock);
        printf("[HIREDIS] WARN : %s\n", buff);
    }
    return 0;
}
template<typename... Args>
int CLog::LogError(const char *vFmt, const Args &... args)
{
    char buff[1024];
    sprintf(buff, vFmt, args...);
    {
        std::lock_guard<mutex> lock(m_logLock);
        printf("[HIREDIS] ERROR : %s\n", buff);
    }
    return 0;
}
template<typename... Args>
int CLog::LogCritical(const char *vFmt, const Args &... args)
{
    char buff[1024];
    sprintf(buff, vFmt, args...);
    {
        std::lock_guard<mutex> lock(m_logLock);
        printf("[HIREDIS] CRITICAL : %s\n", buff);
    }
    return 0;
}

template<typename... Args>
int CLog::Log(CLog::eLogLevel vPriority, const char *vFmt, const Args &... args)
{
    switch (vPriority) {
        case (eLogLevel::TRACE): LogTrace(vFmt, args...);
            break;
        case (eLogLevel::DEBUG): LogDebug(vFmt, args...);
            break;
        case (eLogLevel::INFO): LogInfo(vFmt, args...);
            break;
        case (eLogLevel::WARNING): LogWarn(vFmt, args...);
            break;
        case (eLogLevel::ERROR): LogError(vFmt, args...);
            break;
        case (eLogLevel::CRITICAL): LogCritical(vFmt, args...);
            break;
    }
    return 0;
}

#define HIREDIS_LOG_TRACE exhiredis::CLog::GetSingletonPtr()->LogTrace
#define HIREDIS_LOG_DEBUG exhiredis::CLog::GetSingletonPtr()->LogDebug
#define HIREDIS_LOG_INFO exhiredis::CLog::GetSingletonPtr()->LogInfo
#define HIREDIS_LOG_WARN exhiredis::CLog::GetSingletonPtr()->LogWarn
#define HIREDIS_LOG_ERROR exhiredis::CLog::GetSingletonPtr()->LogError
#define HIREDIS_LOG_CRITICAL exhiredis::CLog::GetSingletonPtr()->LogCritical
}
#endif //EXHIREDIS_LOG_HPP
