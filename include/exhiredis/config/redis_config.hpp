//
// Created by dguco on 18-11-7.
// redis config
//

#ifndef EXHIREDIS_CONFIG_HPP
#define EXHIREDIS_CONFIG_HPP

#include <memory>
#include "base_config.hpp"
#include "master_slave_config.hpp"
#include "sentinel_servers_config.hpp"
#include "single_server_config.hpp"

using namespace std;
namespace exhiredis
{

const int REDIS_MAX_KEY_VALUE_SIZE = 1024 * 1024 * 5;

class CRedisConfig
{
public:
    CRedisConfig(const shared_ptr<MasterSlaveServersConfig> &m_pMasterSlaveConfig,
                 const shared_ptr<CSingleServerConfig> &m_pSingleServerConfig,
                 const shared_ptr<CSentinelServersConfig> &m_pSentinelServerConfig,
                 int redisMaxKeyValueSize = 1024 * 1024 * 5 /*5MB*/)
        : m_pMasterSlaveConfig(m_pMasterSlaveConfig),
          m_pSingleServerConfig(m_pSingleServerConfig),
          m_pSentinelServerConfig(m_pSentinelServerConfig)
    {
        *(const_cast<int *>(&REDIS_MAX_KEY_VALUE_SIZE)) = redisMaxKeyValueSize;
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
//        if (m_pSentinelServerConfig != nullptr) {
//            return static_pointer_cast<CBaseConfig>(m_pSentinelServerConfig);
//        }
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
