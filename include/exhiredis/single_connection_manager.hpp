//
// Created by dguco on 18-11-7.
// redis master slave manager
//

#ifndef EXHIREDIS_MASTERSLAVEMANAGER_HPP
#define EXHIREDIS_MASTERSLAVEMANAGER_HPP

#include <memory>
#include "config/redis_config.hpp"
#include "connection_manager.hpp"
#include "master_slave_entry.hpp"
#include "command_executor_service.hpp"

using namespace std;

namespace exhiredis
{
class CSingleConnectionManager: public IConnectionManager,
                                enable_shared_from_this<CSingleConnectionManager>
{
public:
    //构造函数
    CSingleConnectionManager(const shared_ptr<CRedisConfig> &m_pRedisConfig);
    //初始化
    void Init();
    int CalcSlot(string key) override;
    int CalcSlot(char *key, int len) override;
    shared_ptr<MasterSlaveServersConfig>& GetConfig() override;
    shared_ptr<CMasterSlaveEntry>& GetEntry(int slot) override;
    void ShutDown() override;
    bool IsShutDown() override;
    shared_ptr<CRedisConfig> GetRedisConfig() override;
    shared_ptr<CCommandExecutorService>& GetCommandExecutorService() override;
private:
    static const int MAX_SLOT;
    shared_ptr<CRedisConfig> m_pRedisConfig;
    shared_ptr<CConnectionPool> m_pConnectionPool;
    shared_ptr<CCommandExecutorService> m_pCommandExecutorService;
};

const int CSingleConnectionManager::MAX_SLOT = 16384;

CSingleConnectionManager::CSingleConnectionManager(const shared_ptr<CRedisConfig> &m_pRedisConfig)
    : m_pRedisConfig(m_pRedisConfig)
{

}

void CSingleConnectionManager::Init()
{
    m_pConnectionPool = make_shared<CConnectionPool>(shared_from_this());
    m_pConnectionPool->InitPool(m_pRedisConfig->GetSingleServerConfig()->GetConnectionPoolSize());
    m_pCommandExecutorService = make_shared<CCommandExecutorService>(shared_from_this());
}

shared_ptr<CCommandExecutorService>& CSingleConnectionManager::GetCommandExecutorService()
{
    return m_pCommandExecutorService;
}

int CSingleConnectionManager::CalcSlot(string key)
{
    return 0;
}

int CSingleConnectionManager::CalcSlot(char *key, int len)
{
    return 0;
}

shared_ptr<MasterSlaveServersConfig>& CSingleConnectionManager::GetConfig()
{
    return m_pRedisConfig->GetMasterSlaveConfig();
}

shared_ptr<CMasterSlaveEntry>& CSingleConnectionManager::GetEntry(int slot)
{

}

void CSingleConnectionManager::ShutDown()
{

}

bool CSingleConnectionManager::IsShutDown()
{
    return false;
}

shared_ptr<CRedisConfig> CSingleConnectionManager::GetRedisConfig()
{
    return m_pRedisConfig;
}
}
#endif //EXHIREDIS_MASTERSLAVEMANAGER_HPP
