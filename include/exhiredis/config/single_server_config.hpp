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
                        const string &m_sAddress)
        : CBaseConfig(sPassword),
          m_sAddress(m_sAddress),
          m_iSubscriptionConnectionMinimumIdleSize(1),
          m_iSubscriptionConnectionPoolSize(50),
          m_iConnectionMinimumIdleSize(32),
          m_iConnectionPoolSize(64),
          m_iDatabase(0),
          m_iDnsMonitoringInterval(5000)
    {}

    const string &getM_sAddress() const
    {
        return m_sAddress;
    }
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
private:
    string m_sAddress;
    int m_iSubscriptionConnectionMinimumIdleSize;
    int m_iSubscriptionConnectionPoolSize;
    int m_iConnectionMinimumIdleSize;
    int m_iConnectionPoolSize;
    int m_iDatabase;
    int m_iDnsMonitoringInterval;
};
}
#endif //EXHIREDIS_SINGLE_SERVER_CONFIG_HPP
