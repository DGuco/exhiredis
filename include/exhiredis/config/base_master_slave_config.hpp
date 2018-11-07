//
// Created by dguco on 18-10-18.
//

#ifndef EXHIREDIS_BASE_MASTER_SLAVE_CONFIG_HPP
#define EXHIREDIS_BASE_MASTER_SLAVE_CONFIG_HPP
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
    CBaseMasterSlaveConfig(const string sPassword)
        : CBaseConfig(sPassword),
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
