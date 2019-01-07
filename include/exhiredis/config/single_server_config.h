//
// Created by dguco on 18-10-18.
//

#ifndef EXHIREDIS_SINGLE_SERVER_CONFIG_HPP
#define EXHIREDIS_SINGLE_SERVER_CONFIG_HPP
#include <string>
#include "base_config.h"
using namespace std;
namespace exhiredis
{
class CSingleServerConfig: public CBaseConfig
{
public:
    CSingleServerConfig(const string &address,
                        int port,
                        const string &password)
        : CBaseConfig(address, port, password),
          m_iSubscriptionConnectionMinimumIdleSize(1),
          m_iSubscriptionConnectionPoolSize(50),
          m_iConnectionMinimumIdleSize(32),
          m_iConnectionPoolSize(64),
          m_iDatabase(0),
          m_iDnsMonitoringInterval(5000)
    {}

    int GetSubscriptionConnectionMinimumIdleSize() const
    {
        return m_iSubscriptionConnectionMinimumIdleSize;
    }
    int GetSubscriptionConnectionPoolSize() const
    {
        return m_iSubscriptionConnectionPoolSize;
    }
    int GetConnectionMinimumIdleSize() const
    {
        return m_iConnectionMinimumIdleSize;
    }
    int GetConnectionPoolSize() const
    {
        return m_iConnectionPoolSize;
    }
    int GetDatabase() const
    {
        return m_iDatabase;
    }
    int GetDnsMonitoringInterval() const
    {
        return m_iDnsMonitoringInterval;
    }

    void SetSubscriptionConnectionMinimumIdleSize(int m_iSubscriptionConnectionMinimumIdleSize)
    {
        CSingleServerConfig::m_iSubscriptionConnectionMinimumIdleSize = m_iSubscriptionConnectionMinimumIdleSize;
    }
    void SetSubscriptionConnectionPoolSize(int m_iSubscriptionConnectionPoolSize)
    {
        CSingleServerConfig::m_iSubscriptionConnectionPoolSize = m_iSubscriptionConnectionPoolSize;
    }
    void SetConnectionMinimumIdleSize(int m_iConnectionMinimumIdleSize)
    {
        CSingleServerConfig::m_iConnectionMinimumIdleSize = m_iConnectionMinimumIdleSize;
    }
    void SetConnectionPoolSize(int m_iConnectionPoolSize)
    {
        CSingleServerConfig::m_iConnectionPoolSize = m_iConnectionPoolSize;
    }
private:
    int m_iSubscriptionConnectionMinimumIdleSize;
    int m_iSubscriptionConnectionPoolSize;
    int m_iConnectionMinimumIdleSize;
    int m_iConnectionPoolSize;
    int m_iDatabase;
    int m_iDnsMonitoringInterval;
};
}
#endif //EXHIREDIS_SINGLE_SERVER_CONFIG_HPP
