//
// Created by dguco on 18-10-20.
//

#ifndef EXHIREDIS_LOG_HPP
#define EXHIREDIS_LOG_HPP

#include <mutex>
#include "singleton.h"

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
#define HIREDIS_LOG_TRACE exhiredis::CLog::GetSingletonPtr()->LogTrace
#define HIREDIS_LOG_DEBUG exhiredis::CLog::GetSingletonPtr()->LogDebug
#define HIREDIS_LOG_INFO exhiredis::CLog::GetSingletonPtr()->LogInfo
#define HIREDIS_LOG_WARN exhiredis::CLog::GetSingletonPtr()->LogWarn
#define HIREDIS_LOG_ERROR exhiredis::CLog::GetSingletonPtr()->LogError
#define HIREDIS_LOG_CRITICAL exhiredis::CLog::GetSingletonPtr()->LogCritical
	
#include "../../../sources/utils/log.cpp"
}
#endif //EXHIREDIS_LOG_HPP
