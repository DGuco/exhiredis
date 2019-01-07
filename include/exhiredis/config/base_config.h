//
// Created by dguco on 18-10-18.
// redis base config
//

#ifndef EXHIREDIS_BASE_CONFIG_HPP
#define EXHIREDIS_BASE_CONFIG_HPP

#include <string>
#include <utility>

using namespace std;

namespace exhiredis
{
class CBaseConfig
{
public:
    CBaseConfig(string address, int port, string password)
        : m_sHost(address),
          m_iPort(port),
          m_sPassword(password),
          m_iPingConnectionInterval(0),
          m_bKeepAlive(false),
          m_bTcpNoDelay(false),
          m_iIdleConnectionTimeout(10000),
          m_iPingTimeout(1000),
          m_iConnectTimeout(10000),
          m_iTimeout(3000),
          m_iRetryAttempts(3),
          m_iRetryInterval(1500),
          m_iSubscriptionsPerConnection(5)
    {}

    const string &GetPassword() const
    {
        return m_sPassword;
    }

    int GetPort() const
    {
        return m_iPort;
    }

    const string &GetAddress() const
    {
        return m_sHost;
    }
    int GetPingConnectionInterval() const
    {
        return m_iPingConnectionInterval;
    }
    bool IsKeepAlive() const
    {
        return m_bKeepAlive;
    }
    bool IsTtcpNoDelay() const
    {
        return m_bTcpNoDelay;
    }
    int GetIdleConnectionTimeout() const
    {
        return m_iIdleConnectionTimeout;
    }
    int GetPingTimeout() const
    {
        return m_iPingTimeout;
    }
    int GetConnectTimeout() const
    {
        return m_iConnectTimeout;
    }
    int GetTimeout() const
    {
        return m_iTimeout;
    }
    int GetRetryAttempts() const
    {
        return m_iRetryAttempts;
    }
    int GetRetryInterval() const
    {
        return m_iRetryInterval;
    }
    int GetSubscriptionsPerConnection() const
    {
        return m_iSubscriptionsPerConnection;
    }

    void SetPassword(const string &sPassword)
    {
        m_sPassword = sPassword;
    }

    void SetClientName(const string &sClientName)
    {
        m_sHost = sClientName;
    }
    void SetPingConnectionInterval(int iPingConnectionInterval)
    {
        m_iPingConnectionInterval = iPingConnectionInterval;
    }
    void SetKeepAlive(bool bKeepAlive)
    {
        m_bKeepAlive = bKeepAlive;
    }
    void SetTtcpNoDelay(bool bTtcpNoDelay)
    {
        m_bTcpNoDelay = bTtcpNoDelay;
    }
    void SetIdleConnectionTimeout(int iIdleConnectionTimeout)
    {
        m_iIdleConnectionTimeout = iIdleConnectionTimeout;
    }
    void SetPingTimeout(int iPingTimeout)
    {
        m_iPingTimeout = iPingTimeout;
    }
    void SetConnectTimeout(int iConnectTimeout)
    {
        m_iConnectTimeout = iConnectTimeout;
    }
    void SetTimeout(int iTimeout)
    {
        m_iTimeout = iTimeout;
    }
    void SetRetryAttempts(int iRetryAttempts)
    {
        m_iRetryAttempts = iRetryAttempts;
    }
    void SetRetryInterval(int iRetryInterval)
    {
        m_iRetryInterval = iRetryInterval;
    }
    void SetSubscriptionsPerConnection(int iSubscriptionsPerConnection)
    {
        m_iSubscriptionsPerConnection = iSubscriptionsPerConnection;
    }
private:
    string m_sHost;
    int m_iPort;
    string m_sPassword;
    int m_iPingConnectionInterval;
    bool m_bKeepAlive;
    bool m_bTcpNoDelay;
    int m_iIdleConnectionTimeout;
    int m_iPingTimeout;
    int m_iConnectTimeout;
    int m_iTimeout;
    int m_iRetryAttempts;
    int m_iRetryInterval;
    int m_iSubscriptionsPerConnection;
};
}

#endif //EXHIREDIS_BASE_CONFIG_HPP
