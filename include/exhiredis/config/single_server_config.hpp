//
// Created by dguco on 18-10-18.
//

#ifndef EXHIREDIS_SINGLE_SERVER_CONFIG_HPP
#define EXHIREDIS_SINGLE_SERVER_CONFIG_HPP
#include <string>
#include "base_config.hpp"
using namespace std;
namespace exhiredis
{
	class CSingleServerConfig: public CBaseConfig
	{
	public:
		CSingleServerConfig(const string &sPassword,
							const string &sClientName,
							int iPingConnectionInterval,
							bool bKeepAlive,
							bool bTcpNoDelay,
							string sAddress)
			: CBaseConfig(sPassword, sClientName, iPingConnectionInterval, bKeepAlive, bTcpNoDelay),
			  m_sAddress(std::move(sAddress)),
			  m_iSubscriptionConnectionMinimumIdleSize(1),
			  m_iSubscriptionConnectionPoolSize(50),
			  m_iConnectionMinimumIdleSize(32),
			  m_iConnectionPoolSize(64),
			  m_iDatabase(0),
			  m_lDnsMonitoringInterval(5000)
		{}
	private:
		string m_sAddress;
		int m_iSubscriptionConnectionMinimumIdleSize;
		int m_iSubscriptionConnectionPoolSize;
		int m_iConnectionMinimumIdleSize;
		int m_iConnectionPoolSize;
		int m_iDatabase;
		long m_lDnsMonitoringInterval;
	};
}
#endif //EXHIREDIS_SINGLE_SERVER_CONFIG_HPP
