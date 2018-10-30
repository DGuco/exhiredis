//
// Created by dguco on 18-10-18.
//

#ifndef EXHIREDIS_BASE_MASTER_SLAVE_CONFIG_HPP
#define EXHIREDIS_BASE_MASTER_SLAVE_CONFIG_HPP
#include "exhiredis/config/base_config.hpp"
#include "base_config.hpp"

namespace exhiredis
{
	enum eReadMode
	{
		SLAVE,
		MASTER,
		MASTER_SLAVE,
	};

	enum eSubscriptionMode
	{
		SLAVE,
		MASTER,
	};

	class CBaseMasterSlaveConfig: public CBaseConfig
	{
	public:
		CBaseMasterSlaveConfig(const string &sPassword,
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
			  m_iSlaveConnectionMinimumIdleSize(iSlaveConnectionMinimumIdleSize),
			  m_iSlaveConnectionPoolSize(iSlaveConnectionPoolSize),
			  m_iFailedSlaveReconnectionInterval(iFailedSlaveReconnectionInterval),
			  m_iFailedSlaveCheckInterval(iFailedSlaveCheckInterval),
			  m_iMasterConnectionMinimumIdleSize(iMasterConnectionMinimumIdleSize),
			  m_iMasterConnectionPoolSize(iMasterConnectionPoolSize), m_eReadMode(eReadMode),
			  m_eSubscriptionMode(eSubscriptionMode),
			  m_iSubscriptionConnectionMinimumIdleSize(iSubscriptionConnectionMinimumIdleSize),
			  m_iSubscriptionConnectionPoolSize(iSubscriptionConnectionPoolSize),
			  m_lDnsMonitoringInterval(lDnsMonitoringInterval)
		{}

		CBaseMasterSlaveConfig(const string &sPassword,
							   const string &sClientName,
							   int iPingConnectionInterval,
							   bool bKeepAlive,
							   bool bTtcpNoDelay,
							   int iSlaveConnectionMinimumIdleSize)
			: CBaseConfig(sPassword, sClientName, iPingConnectionInterval, bKeepAlive, bTtcpNoDelay),
			  m_iSlaveConnectionMinimumIdleSize(32),
			  m_iSlaveConnectionPoolSize(64),
			  m_iFailedSlaveReconnectionInterval(3000),
			  m_iFailedSlaveCheckInterval(60000),
			  m_iMasterConnectionMinimumIdleSize(32),
			  m_iMasterConnectionPoolSize(64),
			  m_eReadMode(eReadMode::SLAVE),
			  m_eSubscriptionMode(eSubscriptionMode::MASTER),
			  m_iSubscriptionConnectionMinimumIdleSize(1),
			  m_iSubscriptionConnectionPoolSize(50),
			  m_lDnsMonitoringInterval(5000)
		{}

		int GetSlaveConnectionMinimumIdleSize() const
		{
			return m_iSlaveConnectionMinimumIdleSize;
		}
		int GetSlaveConnectionPoolSize() const
		{
			return m_iSlaveConnectionPoolSize;
		}
		int GetFailedSlaveReconnectionInterval() const
		{
			return m_iFailedSlaveReconnectionInterval;
		}
		int GetFailedSlaveCheckInterval() const
		{
			return m_iFailedSlaveCheckInterval;
		}
		int GetMasterConnectionMinimumIdleSize() const
		{
			return m_iMasterConnectionMinimumIdleSize;
		}
		int GetMasterConnectionPoolSize() const
		{
			return m_iMasterConnectionPoolSize;
		}
		eReadMode GetReadMode() const
		{
			return m_eReadMode;
		}
		eSubscriptionMode GetSubscriptionMode() const
		{
			return m_eSubscriptionMode;
		}
		int GetSubscriptionConnectionMinimumIdleSize() const
		{
			return m_iSubscriptionConnectionMinimumIdleSize;
		}
		int GetSubscriptionConnectionPoolSize() const
		{
			return m_iSubscriptionConnectionPoolSize;
		}
		long GetDnsMonitoringInterval() const
		{
			return m_lDnsMonitoringInterval;
		}

		void SetSlaveConnectionMinimumIdleSize(int iSlaveConnectionMinimumIdleSize)
		{
			m_iSlaveConnectionMinimumIdleSize = iSlaveConnectionMinimumIdleSize;
		}
		void SetSlaveConnectionPoolSize(int iSlaveConnectionPoolSize)
		{
			m_iSlaveConnectionPoolSize = iSlaveConnectionPoolSize;
		}
		void SetFailedSlaveReconnectionInterval(int iFailedSlaveReconnectionInterval)
		{
			m_iFailedSlaveReconnectionInterval = iFailedSlaveReconnectionInterval;
		}
		void SetFailedSlaveCheckInterval(int iFailedSlaveCheckInterval)
		{
			m_iFailedSlaveCheckInterval = iFailedSlaveCheckInterval;
		}
		void SetMasterConnectionMinimumIdleSize(int iMasterConnectionMinimumIdleSize)
		{
			m_iMasterConnectionMinimumIdleSize = iMasterConnectionMinimumIdleSize;
		}
		void SetMasterConnectionPoolSize(int iMasterConnectionPoolSize)
		{
			m_iMasterConnectionPoolSize = iMasterConnectionPoolSize;
		}
		void SetReadMode(eReadMode eReadMode)
		{
			m_eReadMode = eReadMode;
		}
		void SetSubscriptionMode(eSubscriptionMode eSubscriptionMode)
		{
			m_eSubscriptionMode = eSubscriptionMode;
		}
		void SetSubscriptionConnectionMinimumIdleSize(int iSubscriptionConnectionMinimumIdleSize)
		{
			m_iSubscriptionConnectionMinimumIdleSize = iSubscriptionConnectionMinimumIdleSize;
		}
		void SetSubscriptionConnectionPoolSize(int iSubscriptionConnectionPoolSize)
		{
			m_iSubscriptionConnectionPoolSize = iSubscriptionConnectionPoolSize;
		}
		void SetDnsMonitoringInterval(long lDnsMonitoringInterval)
		{
			m_lDnsMonitoringInterval = lDnsMonitoringInterval;
		}
	public:
		int m_iSlaveConnectionMinimumIdleSize;
		int m_iSlaveConnectionPoolSize;
		int m_iFailedSlaveReconnectionInterval;
		int m_iFailedSlaveCheckInterval;
		int m_iMasterConnectionMinimumIdleSize;
		int m_iMasterConnectionPoolSize;
		eReadMode m_eReadMode;
		eSubscriptionMode m_eSubscriptionMode;
		int m_iSubscriptionConnectionMinimumIdleSize;
		int m_iSubscriptionConnectionPoolSize;
		long m_lDnsMonitoringInterval;
	};
}

#endif //EXHIREDIS_BASE_MASTER_SLAVE_CONFIG_HPP
