//
// Created by dguco on 18-10-17.
//

#ifndef EXHIREDIS_CONNECT_MANAGER_H
#define EXHIREDIS_CONNECT_MANAGER_H

#include <memory>
#include <string>
using namespace std;
namespace exhiredis
{
class CRedisNode;
class MasterSlaveServersConfig;
class CRedisConfig;
class CCommandExecutorService;

class IConnectionManager: public enable_shared_from_this<IConnectionManager>
{
public:
    virtual shared_ptr<CCommandExecutorService> &GetCommandExecutorService() = 0;
    virtual shared_ptr<CRedisConnection> GetRedisConnection();
    //calculate the hash slot of the key
    virtual int CalcSlot(string key) = 0;
    //calculate the hash slot of the key
    virtual int CalcSlot(char *key, int len) = 0;
    //get config
    virtual shared_ptr<MasterSlaveServersConfig> &GetConfig() = 0;
    //shutdown
    virtual void ShutDown() = 0;
    //is shutdown
    virtual bool IsShutDown() = 0;
    //get config
    virtual shared_ptr<CRedisConfig> GetRedisConfig() = 0;
};
}
#endif //EXHIREDIS_CONNECT_MANAGER_H
