//
// Created by dguco on 18-11-8.
// redis entry
//

#ifndef EXHIREDIS_REDIS_ENTRY_HPP
#define EXHIREDIS_REDIS_ENTRY_HPP

#include <memory>
#include "comman_def.h"
#include "connection_manager.h"

using namespace std;

namespace exhiredis
{
class CConnectionPool;
class CRedisNode
{
public:

    CRedisNode(shared_ptr<CConnectionPool> &m_pConnectionPool,
               shared_ptr<IConnectionManager> &m_pConnectionManager,
               eRedisNodeType nodeType);
private:
    shared_ptr<CConnectionPool> m_pConnectionPool;
    weak_ptr<IConnectionManager> m_pConnectionManager;
    eRedisNodeType m_nodeType;
};
}
#endif //EXHIREDIS_REDIS_ENTRY_HPP
