//
// Created by dguco on 18-10-20.
// exhiredis log
//

#ifndef EXHIREDIS_LOG_HPP
#define EXHIREDIS_LOG_HPP

#include <mutex>

using namespace std;
namespace exhiredis
{
class CLog
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
    CLog() = default;
    ~CLog() = default;
    template<typename ... Args>
    static int LogTrace(const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogDebug(const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogInfo(const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogWarn(const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogError(const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogCritical(const char *vFmt, const Args &... args);
public:
    template<typename... Args>
    int Log(eLogLevel vPriority, const char *vFmt, const Args &... args);
private:
    static mutex *m_logLock;
};

template<typename... Args>
int CLog::LogTrace(const char *vFmt, const Args &... args)
{
    char buff[1024];
    sprintf(buff, vFmt, args...);
    {
        lock_guard<mutex> lock(*m_logLock);
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
        lock_guard<mutex> lock(*m_logLock);
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
        lock_guard<mutex> lock(*m_logLock);
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
        lock_guard<mutex> lock(*m_logLock);
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
        lock_guard<mutex> lock(*m_logLock);
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
        lock_guard<mutex> lock(*m_logLock);
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

#define HIREDIS_LOG_TRACE CLog::LogTrace
#define HIREDIS_LOG_DEBUG CLog::LogDebug
#define HIREDIS_LOG_INFO CLog::LogInfo
#define HIREDIS_LOG_WARN  CLog::LogWarn
#define HIREDIS_LOG_ERROR CLog::LogError
#define HIREDIS_LOG_CRITICAL CLog::LogCritical
//
//template<typename... Args>
//int logTrace(const char *vFmt, const Args &... args);
//template<typename... Args>
//int logDebug(const char *vFmt, const Args &... args);
//template<typename... Args>
//int logInfo(const char *vFmt, const Args &... args);
//template<typename... Args>
//int logWarn(const char *vFmt, const Args &... args);
//template<typename... Args>
//int logError(const char *vFmt, const Args &... args);
//template<typename... Args>
//int logCritical(const char *vFmt, const Args &... args);
//}
}
#endif //EXHIREDIS_LOG_HPP
