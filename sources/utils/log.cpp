//
// Created by dguco on 18-10-20.
//

#include <mutex>

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
	switch(vPriority){
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
