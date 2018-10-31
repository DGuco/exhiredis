//
// Created by dguco on 18-10-31.
// redis connection pool
//

#ifndef EXHIREDIS_CONNECTION_POOL_HPP
#define EXHIREDIS_CONNECTION_POOL_HPP

#include <memory>
#include <list>
#include <utility>
#include "connection_manager.hpp"
#include "redis_connection.hpp"

using namespace std;
namespace exhiredis
{
	class CConnectionPool
	{
	public:
		CConnectionPool(shared_ptr<IConnectionManager> pConnectionManager);
	private:
		std::list<std::shared_ptr<CRedisConnection>> m_connList;
		std::weak_ptr<IConnectionManager> m_pConnectionManager;
	};

	CConnectionPool::CConnectionPool(shared_ptr<IConnectionManager> pConnectionManager)
		: m_pConnectionManager(pConnectionManager)
	{

	}
}
#endif //EXHIREDIS_CONNECTION_POOL_HPP
