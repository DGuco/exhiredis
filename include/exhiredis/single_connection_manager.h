//
// Created by dguco on 18-11-7.
// redis master slave manager
//

#ifndef EXHIREDIS_MASTERSLAVEMANAGER_HPP
#define EXHIREDIS_MASTERSLAVEMANAGER_HPP

#include <memory>
#include "exhiredis/config/redis_config.h"
#include "connection_manager.h"

using namespace std;

namespace exhiredis
{
class CConnectionPool;
class CCommandExecutorService;
class CSingleConnectionManager: public IConnectionManager
{
public:
    //构造函数
    CSingleConnectionManager(const shared_ptr<CRedisConfig> &m_pRedisConfig);
    //初始化
    void Init();
    int CalcSlot(string key) override;
    int CalcSlot(char *key, int len) override;
    shared_ptr<MasterSlaveServersConfig> &GetConfig() override;
    void ShutDown() override;
    bool IsShutDown() override;
    shared_ptr<CRedisConfig> GetRedisConfig() override;
    shared_ptr<CCommandExecutorService> &GetCommandExecutorService() override;
    shared_ptr<CRedisAsyncConnection> GetRedisConnection() override;
private:
    shared_ptr<CRedisConfig> m_pRedisConfig;
    shared_ptr<CConnectionPool> m_pConnectionPool;
    shared_ptr<CCommandExecutorService> m_pCommandExecutorService;
};
}
#endif //EXHIREDIS_MASTERSLAVEMANAGER_HPP
