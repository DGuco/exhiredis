//
// Created by dguco on 18-10-17.
//

#ifndef EXHIREDIS_CONNECT_MANAGER_H
#define EXHIREDIS_CONNECT_MANAGER_H

namespace exhiredis
{
	class IConnectionManager
	{
	public:
		virtual void shutDown() = 0;
		virtual bool isShubDown() = 0;
	};
}
#endif //EXHIREDIS_CONNECT_MANAGER_H
