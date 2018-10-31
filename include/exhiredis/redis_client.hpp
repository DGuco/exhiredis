//
// Created by dguco on 18-10-31.
//

#ifndef EXHIREDIS_REDISSON_CLIENT_HPP
#define EXHIREDIS_REDISSON_CLIENT_HPP

#include <memory>
#include "connection_manager.hpp"

using namespace std;

namespace exhiredis
{
	class CRedisClient
	{
	private:
		std::shared_ptr<IConnectionManager> m_pConnectionManager;
	};
}
#endif //EXHIREDIS_REDISSON_CLIENT_HPP
