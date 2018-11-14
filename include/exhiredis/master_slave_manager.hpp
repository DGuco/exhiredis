//
// Created by dguco on 18-11-7.
// redis master slave manager
//

#ifndef EXHIREDIS_MASTERSLAVEMANAGER_HPP
#define EXHIREDIS_MASTERSLAVEMANAGER_HPP

#include <memory>
#include "connection_manager.hpp"
#include "master_slave_entry.hpp"
#include "config/redis_config.hpp"

#include <array>

namespace exhiredis
{
class CMasterSlaveManager: public IConnectionManager
{
public:
    CMasterSlaveManager(const shared_ptr<CRedisConfig> &m_pRedisConfig);
    int CalcSlot(string key) override;
    int CalcSlot(char *key, int len) override;
    shared_ptr<MasterSlaveServersConfig> GetConfig() override;
    shared_ptr<CMasterSlaveEntry> GetEntry(int slot) override;
    void ShutDown() override;
    bool IsShutDown() override;
    shared_ptr<CRedisConfig> GetRedisConfig() override;
private:
    static const int MAX_SLOT;
    shared_ptr<CRedisConfig> m_pRedisConfig;
    array<shared_ptr<CMasterSlaveEntry>, MAX_SLOT> m_aSlot2Entry;
};

const int CMasterSlaveManager::MAX_SLOT = 16384;

CMasterSlaveManager::CMasterSlaveManager(const shared_ptr<CRedisConfig> &m_pRedisConfig)
    : m_pRedisConfig(m_pRedisConfig)
{

}

int CMasterSlaveManager::CalcSlot(string key)
{
    return 0;
}

int CMasterSlaveManager::CalcSlot(char *key, int len)
{
    return 0;
}

shared_ptr<MasterSlaveServersConfig> CMasterSlaveManager::GetConfig()
{
    return m_pRedisConfig->GetMasterSlaveConfig();
}

shared_ptr<CMasterSlaveEntry> CMasterSlaveManager::GetEntry(int slot)
{
    if (slot < 0 || slot >= m_aSlot2Entry.size()) {
        HIREDIS_LOG_ERROR("slot is illegal,slot = %d\n", slot);
        return nullptr;
    }
    return m_aSlot2Entry[slot];
}

void CMasterSlaveManager::ShutDown()
{
    for (auto it : m_aSlot2Entry) {
    }
}

bool CMasterSlaveManager::IsShutDown()
{
    return false;
}

shared_ptr<CRedisConfig> CMasterSlaveManager::GetRedisConfig()
{
    return m_pRedisConfig;
}
}
#endif //EXHIREDIS_MASTERSLAVEMANAGER_HPP
