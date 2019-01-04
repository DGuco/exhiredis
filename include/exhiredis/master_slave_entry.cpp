//
// Created by dguco on 19-1-4.
//
#include "master_slave_entry.h"

namespace exhiredis
{
CMasterSlaveEntry::CMasterSlaveEntry(shared_ptr<CConnectionPool> &m_pConnectionPool,
                                     shared_ptr<IConnectionManager> &m_pConnectionManager)
    : m_pConnectionPool(m_pConnectionPool),
      m_pConnectionManager(m_pConnectionManager)
{
}
}