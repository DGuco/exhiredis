//
// Created by dguco on 18-11-7.
// redis config
//

#ifndef EXHIREDIS_CONFIG_HPP
#define EXHIREDIS_CONFIG_HPP

#include <memory>
#include "master_slave_config.hpp"
#include "sentinel_servers_config.hpp"
#include "single_server_config.hpp"

using namespace std;
namespace exhiredis
{
class CRedisConfig
{
public:
    CRedisConfig(const shared_ptr<MasterSlaveServersConfig> &m_pMasterSlaveConfig,
                 const shared_ptr<CSingleServerConfig> &m_pSingleServerConfig,
                 const shared_ptr<CSentinelServersConfig> &m_pSentinelServerConfig)
        : m_pMasterSlaveConfig(m_pMasterSlaveConfig),
          m_pSingleServerConfig(m_pSingleServerConfig),
          m_pSentinelServerConfig(m_pSentinelServerConfig)
    {}

    shared_ptr<MasterSlaveServersConfig> GetMasterSlaveConfig() const
    {
        return m_pMasterSlaveConfig;
    }

    shared_ptr<CSingleServerConfig> GetSingleServerConfig() const
    {
        return m_pSingleServerConfig;
    }

    shared_ptr<CSentinelServersConfig> GetSentinelServerConfig() const
    {
        return m_pSentinelServerConfig;
    }

private:
    /**
     * There is only one should be not null at the same time
     */
    shared_ptr<MasterSlaveServersConfig> m_pMasterSlaveConfig;
    shared_ptr<CSingleServerConfig> m_pSingleServerConfig;
    shared_ptr<CSentinelServersConfig> m_pSentinelServerConfig;
};
}
#endif //EXHIREDIS_CONFIG_HPP
