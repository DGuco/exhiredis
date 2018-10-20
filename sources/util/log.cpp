//
// Created by dguco on 18-10-20.
//

#include <mutex>
#include "utils/log.h"

namespace exhiredis
{
	template<typename... Args>
	int CLog::LogTrace(const char *vFmt, const Args &... args)
	{
		return 0;
	}
	template<typename... Args>
	int CLog::LogDebug(const char *vFmt, const Args &... args)
	{
		return 0;
	}
	template<typename... Args>
	int CLog::LogInfo(const char *vFmt, const Args &... args)
	{
		return 0;
	}
	template<typename... Args>
	int CLog::LogWarn(const char *vFmt, const Args &... args)
	{
		return 0;
	}
	template<typename... Args>
	int CLog::LogError(const char *vFmt, const Args &... args)
	{
		return 0;
	}
	template<typename... Args>
	int CLog::LogCritical(const char *vFmt, const Args &... args)
	{
		return 0;
	}
	template<typename... Args>
	int CLog::Log(CLog::eLogLevel vPriority, const char *vFmt, const Args &... args)
	{
		std::lock_guard<mutex> lock(m_logLock);
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
}