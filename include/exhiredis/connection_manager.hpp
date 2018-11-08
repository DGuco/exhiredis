//
// Created by dguco on 18-10-17.
//

#ifndef EXHIREDIS_CONNECT_MANAGER_H
#define EXHIREDIS_CONNECT_MANAGER_H

#include <memory>
#include "config/redis_config.hpp"

using namespace std;

namespace exhiredis
{
class IConnectionManager
{
public:
    virtual void ShutDown() = 0;
    virtual bool IsShubDown() = 0;
    virtual shared_ptr<CRedisConfig> &GetRedisConfig() = 0;
};
}
#endif //EXHIREDIS_CONNECT_MANAGER_H
