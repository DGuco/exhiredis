//
// Created by dguco on 18-11-8.
// redis entry
//

#ifndef EXHIREDIS_REDIS_ENTRY_HPP
#define EXHIREDIS_REDIS_ENTRY_HPP

#include "connection_pool.hpp"

namespace exhiredis
{
enum class enEntryModule
{
    ENTRY_MASTER,
    ENTRY_SLAVE
};

class CRedisEntry
{
public:
    CRedisEntry(enEntryModule m_eEntryModule,
                const shared_ptr<CConnectionPool> &m_pConnectionPool);
private:
    enEntryModule m_eEntryModule;
    shared_ptr<CConnectionPool> m_pConnectionPool;
};

CRedisEntry::CRedisEntry(enEntryModule m_eEntryModule,
                         const shared_ptr<CConnectionPool> &m_pConnectionPool)
    : m_eEntryModule(m_eEntryModule),
      m_pConnectionPool(m_pConnectionPool)
{}

}
#endif //EXHIREDIS_REDIS_ENTRY_HPP
