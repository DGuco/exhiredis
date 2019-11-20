//
// Created by dguco on 19-1-4.
//
#include "redis_node.h"

namespace exhiredis
{
CRedisNode::CRedisNode(shared_ptr<CConnectionPool> &m_pConnectionPool,
                       shared_ptr<IConnectionManager> &m_pConnectionManager,
                       eRedisNodeType nodeType)
    : m_pConnectionPool(m_pConnectionPool),
      m_pConnectionManager(m_pConnectionManager),
      m_nodeType(nodeType)
{
    
}
}