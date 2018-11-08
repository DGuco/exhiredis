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
    CRedisConfig(const shared_ptr<CBaseMasterSlaveConfig> &m_pMasterSlaveConfig,
                 const shared_ptr<CSingleServerConfig> &m_pSingleServerConfig,
                 const shared_ptr<CSentinelServersConfig> &m_pSentinelServerConfig)
        : m_pMasterSlaveConfig(m_pMasterSlaveConfig),
          m_pSingleServerConfig(m_pSingleServerConfig),
          m_pSentinelServerConfig(m_pSentinelServerConfig)
    {}

    const shared_ptr<CBaseMasterSlaveConfig> &getMasterSlaveConfig() const
    {
        return m_pMasterSlaveConfig;
    }

    const shared_ptr<CSingleServerConfig> &getSingleServerConfig() const
    {
        return m_pSingleServerConfig;
    }

    const shared_ptr<CSentinelServersConfig> &getSentinelServerConfig() const
    {
        return m_pSentinelServerConfig;
    }

private:
    /**
     * There is only one should be not null at the same time
     */
    shared_ptr<CBaseMasterSlaveConfig> m_pMasterSlaveConfig;
    shared_ptr<CSingleServerConfig> m_pSingleServerConfig;
    shared_ptr<CSentinelServersConfig> m_pSentinelServerConfig;
};
}
#endif //EXHIREDIS_CONFIG_HPP
