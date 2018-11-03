//
// Created by dguco on 18-10-31.
// single redis connection manager
//

#ifndef EXHIREDIS_SINGLE_CONNECTION_MANAGER_HPP
#define EXHIREDIS_SINGLE_CONNECTION_MANAGER_HPP

#include "connection_manager.hpp"

namespace exhiredis
{
	class CSingleConnectionManager: public IConnectionManager
	{
	public:
		static const int MAX_SLOT = 16384;
	private:
	};
}
#endif //EXHIREDIS_SINGLE_CONNECTION_MANAGER_HPP
