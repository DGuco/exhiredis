//
// Created by dguco on 19-1-4.
//

#ifndef EXHIREDIS_CLUSTER_CONNECTION_MANAGER_H
#define EXHIREDIS_CLUSTER_CONNECTION_MANAGER_H
#include <memory>
#include "exhiredis/config/redis_config.h"
#include "connection_manager.h"

using namespace std;

namespace exhiredis
{
class CCommandExecutorService;
class MasterSlaveServersConfig;
class CRedisNode;
class CRedisConnection;
class CClusterConnectionManager: public IConnectionManager
{
public:
    //构造函数
    CClusterConnectionManager(const shared_ptr<CRedisConfig> &redisConfig);
    void Init();
    shared_ptr<CCommandExecutorService> &GetCommandExecutorService() override;
    int CalcSlot(string key) override;
    int CalcSlot(char *key, int len) override;
    shared_ptr<MasterSlaveServersConfig> &GetConfig() override;
    void ShutDown() override;
    bool IsShutDown() override;
    shared_ptr<CRedisConfig> GetRedisConfig() override;
    shared_ptr<CRedisConnection> GetRedisConnection() override;
private:
    shared_ptr<CRedisConfig> m_pRedisConfig;
    shared_ptr<CCommandExecutorService> m_pCommandExecutorService;
    static const int MAX_SLOT;
};
}
#endif //EXHIREDIS_CLUSTER_CONNECTION_MANAGER_H
