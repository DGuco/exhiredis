//
// Created by dguco on 18-11-7.
// redis master slave manager
//

#ifndef EXHIREDIS_MASTERSLAVEMANAGER_HPP
#define EXHIREDIS_MASTERSLAVEMANAGER_HPP

#include <memory>
#include "connection_manager.hpp"
#include "config/redis_config.hpp"

namespace exhiredis
{
class CMasterSlaveManager: public IConnectionManager
{
public:
    CMasterSlaveManager(const shared_ptr<CRedisConfig> &m_pRedisConfig);
private:
    shared_ptr<CRedisConfig> m_pRedisConfig;
};

CMasterSlaveManager::CMasterSlaveManager(const shared_ptr<CRedisConfig> &m_pRedisConfig)
    : m_pRedisConfig(m_pRedisConfig)
{

}
}
#endif //EXHIREDIS_MASTERSLAVEMANAGER_HPP
