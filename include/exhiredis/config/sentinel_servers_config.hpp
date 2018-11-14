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
                           const list<string> &m_sentinelAddresses,
                           const string &m_sMasterName,
                           int m_iDatabase,
                           int m_iScanInterval)
        : CBaseMasterSlaveConfig(sPassword),
          m_sentinelAddresses(m_sentinelAddresses),
          m_sMasterName(m_sMasterName),
          m_iDatabase(m_iDatabase),
          m_iScanInterval(m_iScanInterval)
    {}

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
    list<string> m_sentinelAddresses;
    string m_sMasterName;
    int m_iDatabase;
    int m_iScanInterval;
};
}
#endif //EXHIREDIS_SENTINEL_SERVERS_CONFIG_HPP
