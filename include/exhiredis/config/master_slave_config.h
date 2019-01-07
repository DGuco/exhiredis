//
// Created by dguco on 18-10-18.
//

#ifndef EXHIREDIS_MASTER_SLAVE_CONFIG_HPP
#define EXHIREDIS_MASTER_SLAVE_CONFIG_HPP

#include <list>
#include "base_master_slave_config.h"

using namespace std;

namespace exhiredis
{
class MasterSlaveServersConfig: public CBaseMasterSlaveConfig
{
public:
    MasterSlaveServersConfig(const string &address,
                             int port,
                             const string &password,
                             const list<string> &m_slaveAddresses,
                             const string &m_sMasterAddress)
        : CBaseMasterSlaveConfig(address,port, password),
          m_slaveAddresses(m_slaveAddresses),
          m_sMasterAddress(m_sMasterAddress),
          database(0)
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
    list<string> m_slaveAddresses;
    string m_sMasterAddress;
    int database;
};
}
#endif //EXHIREDIS_MASTER_SLAVE_CONFIG_HPP
