//
// Created by dguco on 18-11-7.
// redis config
//

#ifndef EXHIREDIS_CONFIG_HPP
#define EXHIREDIS_CONFIG_HPP

#include <memory>
#include "base_config.h"
#include "master_slave_config.h"
#include "sentinel_servers_config.h"
#include "single_server_config.h"

using namespace std;
namespace exhiredis
{
class CRedisConfig
{
public:
    CRedisConfig(shared_ptr<MasterSlaveServersConfig> m_pMasterSlaveConfig,
                 shared_ptr<CSingleServerConfig> m_pSingleServerConfig,
                 shared_ptr<CSentinelServersConfig> m_pSentinelServerConfig)
        : m_pMasterSlaveConfig(m_pMasterSlaveConfig),
          m_pSingleServerConfig(m_pSingleServerConfig),
          m_pSentinelServerConfig(m_pSentinelServerConfig)
    {
    }

    shared_ptr<MasterSlaveServersConfig> &GetMasterSlaveConfig()
    {
        return m_pMasterSlaveConfig;
    }

    shared_ptr<CSingleServerConfig> &GetSingleServerConfig()
    {
        return m_pSingleServerConfig;
    }

    shared_ptr<CSentinelServersConfig> &GetSentinelServerConfig()
    {
        return m_pSentinelServerConfig;
    }

    shared_ptr<CBaseConfig> GetConnConfig()
    {
        if (m_pMasterSlaveConfig != nullptr) {
            return static_pointer_cast<CBaseConfig>(m_pMasterSlaveConfig);
        }
        if (m_pSingleServerConfig != nullptr) {
            return static_pointer_cast<CBaseConfig>(m_pSingleServerConfig);
        }
        if (m_pSentinelServerConfig != nullptr) {
            return static_pointer_cast<CBaseConfig>(m_pSentinelServerConfig);
        }
        return nullptr;
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
