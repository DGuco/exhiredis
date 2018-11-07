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
