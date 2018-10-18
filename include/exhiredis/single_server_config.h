//
// Created by dguco on 18-10-18.
//

#ifndef EXHIREDIS_SINGLE_SERVER_CONFIG_HPP
#define EXHIREDIS_SINGLE_SERVER_CONFIG_HPP

#include "base_config.h"
#include <string>
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
							bool bTtcpNoDelay,
							int iIdleConnectionTimeout,
							int iPingTimeout,
							int iConnectTimeout,
							int iTimeout,
							int iRetryAttempts,
							int iRetryInterval,
							int iSubscriptionsPerConnection,
							const string &sAddress,
							int iSubscriptionConnectionMinimumIdleSize,
							int iSubscriptionConnectionPoolSize,
							int iConnectionMinimumIdleSize,
							int iConnectionPoolSize,
							int iDatabase,
							long lDnsMonitoringInterval)
			: CBaseConfig(sPassword,
						  sClientName,
						  iPingConnectionInterval,
						  bKeepAlive,
						  bTtcpNoDelay,
						  iIdleConnectionTimeout,
						  iPingTimeout,
						  iConnectTimeout,
						  iTimeout,
						  iRetryAttempts,
						  iRetryInterval,
						  iSubscriptionsPerConnection),
			  m_sAddress(sAddress),
			  m_iSubscriptionConnectionMinimumIdleSize(iSubscriptionConnectionMinimumIdleSize),
			  m_iSubscriptionConnectionPoolSize(iSubscriptionConnectionPoolSize),
			  m_iConnectionMinimumIdleSize(iConnectionMinimumIdleSize),
			  m_iConnectionPoolSize(iConnectionPoolSize),
			  m_iDatabase(iDatabase),
			  m_lDnsMonitoringInterval(lDnsMonitoringInterval)
		{}

		CSingleServerConfig(const string &sPassword,
							const string &sClientName,
							int iPingConnectionInterval,
							bool bKeepAlive,
							bool bTtcpNoDelay,
							string sAddress)
			: CBaseConfig(sPassword, sClientName, iPingConnectionInterval, bKeepAlive, bTtcpNoDelay),
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
