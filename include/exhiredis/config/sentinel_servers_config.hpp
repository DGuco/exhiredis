//
// Created by dguco on 18-10-31.
//

#ifndef EXHIREDIS_SENTINEL_SERVERS_CONFIG_HPP
#define EXHIREDIS_SENTINEL_SERVERS_CONFIG_HPP

#include <string>
#include <list>
#include "base_master_slave_config.hpp"

using namespace std;
namespace exhiredis
{
	class CSentinelServersConfig: CBaseMasterSlaveConfig
	{
	public:
		CSentinelServersConfig(const string &sPassword,
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
							   int iSlaveConnectionMinimumIdleSize,
							   int iSlaveConnectionPoolSize,
							   int iFailedSlaveReconnectionInterval,
							   int iFailedSlaveCheckInterval,
							   int iMasterConnectionMinimumIdleSize,
							   int iMasterConnectionPoolSize,
							   eReadMode eReadMode,
							   eSubscriptionMode eSubscriptionMode,
							   int iSubscriptionConnectionMinimumIdleSize,
							   int iSubscriptionConnectionPoolSize,
							   long lDnsMonitoringInterval,
							   const list<string> &sentinelAddresses,
							   const string &masterName,
							   int database,
							   int scanInterval)
			: CBaseMasterSlaveConfig(sPassword,
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
									 iSubscriptionsPerConnection,
									 iSlaveConnectionMinimumIdleSize,
									 iSlaveConnectionPoolSize,
									 iFailedSlaveReconnectionInterval,
									 iFailedSlaveCheckInterval,
									 iMasterConnectionMinimumIdleSize,
									 iMasterConnectionPoolSize,
									 eReadMode,
									 eSubscriptionMode,
									 iSubscriptionConnectionMinimumIdleSize,
									 iSubscriptionConnectionPoolSize,
									 lDnsMonitoringInterval), m_sentinelAddresses(sentinelAddresses),
			  m_sMasterName(masterName), m_iDatabase(database), m_iScanInterval(scanInterval)
		{}

		CSentinelServersConfig(const string &sPassword,
							   const string &sClientName,
							   int iPingConnectionInterval,
							   bool bKeepAlive,
							   bool bTtcpNoDelay,
							   int iSlaveConnectionMinimumIdleSize,
							   const list<string> &sentinelAddresses,
							   const string &masterName,
							   int database,
							   int scanInterval)
			: CBaseMasterSlaveConfig(sPassword,
									 sClientName,
									 iPingConnectionInterval,
									 bKeepAlive,
									 bTtcpNoDelay,
									 iSlaveConnectionMinimumIdleSize), m_sentinelAddresses(sentinelAddresses),
			  m_sMasterName(masterName), m_iDatabase(database), m_iScanInterval(scanInterval)
		{

		}

		const list<string> &GetSentinelAddresses() const
		{
			return m_sentinelAddresses;
		}
		const string &GetMasterName() const
		{
			return m_sMasterName;
		}
		int GetDatabase() const
		{
			return m_iDatabase;
		}
		int GetScanInterval() const
		{
			return m_iScanInterval;
		}

		void SetSentinelAddresses(const list<string> &sentinelAddresses)
		{
			m_sentinelAddresses = sentinelAddresses;
		}
		void SetMasterName(const string &masterName)
		{
			CSentinelServersConfig::m_sMasterName = masterName;
		}
		void SetDatabase(int database)
		{
			CSentinelServersConfig::m_iDatabase = database;
		}
		void SetScanInterval(int scanInterval)
		{
			CSentinelServersConfig::m_iScanInterval = scanInterval;
		}
	private:
		std::list<string> m_sentinelAddresses;
		string m_sMasterName;
		int m_iDatabase;
		int m_iScanInterval;
	};
}
#endif //EXHIREDIS_SENTINEL_SERVERS_CONFIG_HPP
