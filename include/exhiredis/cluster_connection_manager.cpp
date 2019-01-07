//
// Created by dguco on 19-1-4.
//

#include "connection_manager.h"
#include "cluster_connection_manager.h"
#include "command_executor_service.h"
#include "redis_node.h"
#include "exhiredis/utils/crc16.h"

namespace exhiredis
{
const int CClusterConnectionManager::MAX_SLOT = 16384;

CClusterConnectionManager::CClusterConnectionManager(const shared_ptr<CRedisConfig> &redisConfig)
    : m_pRedisConfig(redisConfig)
{

}

void CClusterConnectionManager::Init()
{
    m_pCommandExecutorService =
        make_shared<CCommandExecutorService>(static_pointer_cast<IConnectionManager>(shared_from_this()));
}

shared_ptr<CCommandExecutorService> &CClusterConnectionManager::GetCommandExecutorService()
{
    return m_pCommandExecutorService;
}
int CClusterConnectionManager::CalcSlot(string key)
{
    if (key == "") {
        return 0;
    }

    int start = (int) key.find_first_of('{');
    if (start != key.npos) {
        int end = (int) key.find_first_of('}');
        key = key.substr(start + 1, end);
    }
    int result = CCrc16::GetCrc16(key.c_str(), (int) key.length()) % MAX_SLOT;
    return result;
}

int CClusterConnectionManager::CalcSlot(char *key, int len)
{
    if (key == nullptr || len == 0) {
        return 0;
    }

    string str;
    str.assign(key, len);
    return CalcSlot(key);
}

shared_ptr<MasterSlaveServersConfig> &CClusterConnectionManager::GetConfig()
{
}

void CClusterConnectionManager::ShutDown()
{

}
bool CClusterConnectionManager::IsShutDown()
{
    return false;
}
shared_ptr<CRedisConfig> CClusterConnectionManager::GetRedisConfig()
{
    return nullptr;
}

shared_ptr<CRedisConnection> CClusterConnectionManager::GetRedisConnection()
{
    return nullptr;
}

}