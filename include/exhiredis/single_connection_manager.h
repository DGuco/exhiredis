//
// Created by dguco on 18-11-7.
// redis master slave manager
//

#ifndef EXHIREDIS_MASTERSLAVEMANAGER_HPP
#define EXHIREDIS_MASTERSLAVEMANAGER_HPP

#include <memory>
#include "exhiredis/config/redis_config.h"

using namespace std;

namespace exhiredis
{
class IConnectionManager;
class CSingleConnectionManager;
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
    shared_ptr<CMasterSlaveEntry> &GetEntry(int slot) override;
    void ShutDown() override;
    bool IsShutDown() override;
    shared_ptr<CRedisConfig> GetRedisConfig() override;
    shared_ptr<CCommandExecutorService> &GetCommandExecutorService() override;
private:
    static const int MAX_SLOT;
    shared_ptr<CRedisConfig> m_pRedisConfig;
    shared_ptr<CConnectionPool> m_pConnectionPool;
    shared_ptr<CCommandExecutorService> m_pCommandExecutorService;
};
}
#endif //EXHIREDIS_MASTERSLAVEMANAGER_HPP
