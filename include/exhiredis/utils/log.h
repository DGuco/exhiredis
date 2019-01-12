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
    static int LogTrace(const char *file, int line, const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogDebug(const char *file, int line, const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogInfo(const char *file, int line, const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogWarn(const char *file, int line, const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogError(const char *file, int line, const char *vFmt, const Args &... args);
    template<typename... Args>
    static int LogCritical(const char *file, int line, const char *vFmt, const Args &... args);
public:
    template<typename... Args>
    int Log(eLogLevel vPriority, const char *file, int line, const char *vFmt, const Args &... args);
    template<typename... Args>
    static void FormatLogStr(const char *dest, const char *file, int line, const char *vFmt, const Args &... args);
private:
    static mutex *m_logLock;
};

template<typename... Args>
int CLog::LogTrace(const char *file, int line, const char *vFmt, const Args &... args)
{
    char buff[2048];
    FormatLogStr(buff, file, line, vFmt, args...);
    {
        lock_guard<mutex> lock(*m_logLock);
        printf("[HIREDIS TRACE] %s\n", buff);
    }
    return 0;
}

template<typename... Args>
int CLog::LogDebug(const char *file, int line, const char *vFmt, const Args &... args)
{
    char buff[2048];
    FormatLogStr(buff, file, line, vFmt, args...);
    {
        lock_guard<mutex> lock(*m_logLock);
        printf("[HIREDIS DEBUG] %s\n", buff);
    }
    return 0;
}

template<typename... Args>
int CLog::LogInfo(const char *file, int line, const char *vFmt, const Args &... args)
{
    char buff[2048];
    FormatLogStr(buff, file, line, vFmt, args...);
    {
        lock_guard<mutex> lock(*m_logLock);
        printf("[HIREDIS INFO] %s\n", buff);
    }
    return 0;
}
template<typename... Args>
int CLog::LogWarn(const char *file, int line, const char *vFmt, const Args &... args)
{
    char buff[2048];
    FormatLogStr(buff, file, line, vFmt, args...);
    {
        lock_guard<mutex> lock(*m_logLock);
        printf("[HIREDIS WARN] %s\n", buff);
    }
    return 0;
}
template<typename... Args>
int CLog::LogError(const char *file, int line, const char *vFmt, const Args &... args)
{
    char buff[2048];
    FormatLogStr(buff, file, line, vFmt, args...);
    {
        lock_guard<mutex> lock(*m_logLock);
        printf("[HIREDIS ERROR] %s\n", buff);
    }
    return 0;
}
template<typename... Args>
int CLog::LogCritical(const char *file, int line, const char *vFmt, const Args &... args)
{
    char buff[2048];
    FormatLogStr(buff, file, line, vFmt, args...);
    {
        lock_guard<mutex> lock(*m_logLock);
        printf("[HIREDIS CRITICAL] %s\n", buff);
    }
    return 0;
}

template<typename... Args>
int CLog::Log(CLog::eLogLevel vPriority, const char *file, int line, const char *vFmt, const Args &... args)
{
    switch (vPriority) {
        case (eLogLevel::TRACE): LogTrace(file, line, vFmt, args...);
            break;
        case (eLogLevel::DEBUG): LogDebug(file, line, vFmt, args...);
            break;
        case (eLogLevel::INFO): LogInfo(file, line, vFmt, args...);
            break;
        case (eLogLevel::WARNING): LogWarn(file, line, vFmt, args...);
            break;
        case (eLogLevel::ERROR): LogError(file, line, vFmt, args...);
            break;
        case (eLogLevel::CRITICAL): LogCritical(file, line, vFmt, args...);
            break;
    }
    return 0;
}

template<typename... Args>
void CLog::FormatLogStr(const char *dest, const char *file, int line, const char *vFmt, const Args &... args)
{
    string filestr(file);
    filestr = "[" + filestr + ":" + to_string(line) + "] ";
    string fmt(vFmt);
    fmt = filestr + fmt;
    sprintf((char *) dest, fmt.c_str(), args...);
}

#define HIREDIS_LOG_TRACE(vFmt, args...) CLog::LogTrace(__FILE__, __LINE__,vFmt,  ##args)
#define HIREDIS_LOG_DEBUG(vFmt, args...) CLog::LogDebug(__FILE__, __LINE__,vFmt,  ##args)
#define HIREDIS_LOG_INFO(vFmt, args...) CLog::LogInfo(__FILE__, __LINE__,vFmt,  ##args)
#define HIREDIS_LOG_WARN(vFmt, args...)  CLog::LogWarn(__FILE__, __LINE__,vFmt,  ##args)
#define HIREDIS_LOG_ERROR(vFmt, args...) CLog::LogError(__FILE__, __LINE__,vFmt,  ##args)
#define HIREDIS_LOG_CRITICAL(vFmt, args...) CLog::LogCritical(__FILE__, __LINE__,vFmt,  ##args)
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
