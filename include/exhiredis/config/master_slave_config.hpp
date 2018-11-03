//
// Created by dguco on 18-10-18.
//

#ifndef EXHIREDIS_MASTER_SLAVE_CONFIG_HPP
#define EXHIREDIS_MASTER_SLAVE_CONFIG_HPP

#include <list>
#include "base_master_slave_config.hpp"

using namespace std;

namespace exhiredis
{
	class MasterSlaveServersConfig: public CBaseMasterSlaveConfig
	{
	public:
		MasterSlaveServersConfig(const string &sPassword,
								 const string &sClientName,
								 int iPingConnectionInterval,
								 bool bKeepAlive,
								 bool bTtcpNoDelay,
								 int iSlaveConnectionMinimumIdleSize,
								 list<string> slaveAddresses,
								 string sMasterAddress,
								 int database)
			: CBaseMasterSlaveConfig(sPassword,
									 sClientName,
									 iPingConnectionInterval,
									 bKeepAlive,
									 bTtcpNoDelay,
									 iSlaveConnectionMinimumIdleSize),
			  m_slaveAddresses(std::move(slaveAddresses)),
			  m_sMasterAddress(std::move(sMasterAddress)), database(database)
		{}

		const list<string> &GetSlaveAddresses() const
		{
			return m_slaveAddresses;
		}

		const string &GetMasterAddress() const
		{
			return m_sMasterAddress;
		}

		int GetDatabase() const
		{
			return database;
		}
	private:
		std::list<string> m_slaveAddresses;
		string m_sMasterAddress;
		int database;
	};
}
#endif //EXHIREDIS_MASTER_SLAVE_CONFIG_HPP
