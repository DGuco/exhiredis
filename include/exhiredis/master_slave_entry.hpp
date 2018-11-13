//
// Created by dguco on 18-11-8.
// redis entry
//

#ifndef EXHIREDIS_REDIS_ENTRY_HPP
#define EXHIREDIS_REDIS_ENTRY_HPP

#include "connection_pool.hpp"

namespace exhiredis
{
class CMasterSlaveEntry
{
public:
    CMasterSlaveEntry(shared_ptr<CConnectionPool> m_pConnectionPool,
                      shared_ptr<IConnectionManager> m_pConnectionManager);
private:
    shared_ptr<CConnectionPool> m_pConnectionPool;
    weak_ptr<IConnectionManager> m_pConnectionManager;
};

CMasterSlaveEntry::CMasterSlaveEntry(shared_ptr<CConnectionPool> m_pConnectionPool,
                                     shared_ptr<IConnectionManager> m_pConnectionManager)
    : m_pConnectionPool(m_pConnectionPool),
      m_pConnectionManager(m_pConnectionManager)
{
}
}
#endif //EXHIREDIS_REDIS_ENTRY_HPP
