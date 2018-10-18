//
// Created by dguco on 18-10-18.
//

#ifndef EXHIREDIS_MASTER_SLAVE_CONFIG_HPP
#define EXHIREDIS_MASTER_SLAVE_CONFIG_HPP

#include <hash_set>
#include <list>
#include <utility>
#include "base_master_slave_config.h"

using namespace std;

namespace exhiredis
{
	class MasterSlaveServersConfig: public CBaseMasterSlaveConfig
	{
	public:
		MasterSlaveServersConfig(const string &sPassword,
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
								 list<string> slaveAddresses,
								 string sMasterAddress,
								 int database)
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
									 lDnsMonitoringInterval), m_slaveAddresses(std::move(slaveAddresses)),
			  m_sMasterAddress(std::move(sMasterAddress)), database(database)
		{}

		MasterSlaveServersConfig(const string &sPassword,
								 const string &sClientName,
								 int iPingConnectionInterval,
								 bool bKeepAlive,
								 bool bTtcpNoDelay,
								 int iSlaveConnectionMinimumIdleSize,
								 list<string> slaveAddresses,
								 string sMasterAddress,
								 int database)
			: CBaseMasterSlaveConfig(sPassword,
									 sClientName,
									 iPingConnectionInterval,
									 bKeepAlive,
									 bTtcpNoDelay,
									 iSlaveConnectionMinimumIdleSize),
			  m_slaveAddresses(std::move(slaveAddresses)),
			  m_sMasterAddress(std::move(sMasterAddress)), database(database)
		{}

		const list<string> &GetSlaveAddresses() const
		{
			return m_slaveAddresses;
		}

		const string &GetMasterAddress() const
		{
			return m_sMasterAddress;
		}

		int GetDatabase() const
		{
			return database;
		}
	private:
		std::list<string> m_slaveAddresses;
		string m_sMasterAddress;
		int database;
	};
}
#endif //EXHIREDIS_MASTER_SLAVE_CONFIG_HPP
