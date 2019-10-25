//
// Created by dguco on 19-1-4.
//
#include "connection_manager.h"
#include "single_connection_manager.h"
#include "connection_pool.h"
#include "command_executor_service.h"
namespace exhiredis
{
CSingleConnectionManager::CSingleConnectionManager(const shared_ptr<CRedisConfig> &m_pRedisConfig)
    : m_pRedisConfig(m_pRedisConfig)
{

}

void CSingleConnectionManager::Init()
{
    m_pConnectionPool = make_shared<CConnectionPool>(shared_from_this());
    m_pConnectionPool->InitPool(m_pRedisConfig->GetSingleServerConfig()->GetConnectionPoolSize());
    m_pCommandExecutorService =
        make_shared<CCommandExecutorService>(static_pointer_cast<IConnectionManager>(shared_from_this()));
}

shared_ptr<CCommandExecutorService> &CSingleConnectionManager::GetCommandExecutorService()
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

shared_ptr<MasterSlaveServersConfig> &CSingleConnectionManager::GetConfig()
{
    return m_pRedisConfig->GetMasterSlaveConfig();
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

shared_ptr<CRedisAsyncConnection> CSingleConnectionManager::GetRedisConnection()
{
    return m_pConnectionPool->BorrowOneConnection();
}
}