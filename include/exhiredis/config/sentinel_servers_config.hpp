//
// Created by dguco on 18-10-31.
//

#ifndef EXHIREDIS_SENTINEL_SERVERS_CONFIG_HPP
#define EXHIREDIS_SENTINEL_SERVERS_CONFIG_HPP

#include <string>
#include <list>
#include "base_master_slave_config.hpp"

using namespace std;
namespace exhiredis
{
	class CSentinelServersConfig: CBaseMasterSlaveConfig
	{
	public:
		CSentinelServersConfig(const string &sPassword,
							   const string &sClientName,
							   int iPingConnectionInterval,
							   bool bKeepAlive,
							   bool bTtcpNoDelay,
							   int iSlaveConnectionMinimumIdleSize,
							   const list<string> &sentinelAddresses,
							   const string &masterName,
							   int database,
							   int scanInterval)
			: CBaseMasterSlaveConfig(sPassword,
									 sClientName,
									 iPingConnectionInterval,
									 bKeepAlive,
									 bTtcpNoDelay,
									 iSlaveConnectionMinimumIdleSize), m_sentinelAddresses(sentinelAddresses),
			  m_sMasterName(masterName), m_iDatabase(database), m_iScanInterval(scanInterval)
		{

		}

		const list<string> &GetSentinelAddresses() const
		{
			return m_sentinelAddresses;
		}
		const string &GetMasterName() const
		{
			return m_sMasterName;
		}
		int GetDatabase() const
		{
			return m_iDatabase;
		}
		int GetScanInterval() const
		{
			return m_iScanInterval;
		}

		void SetSentinelAddresses(const list<string> &sentinelAddresses)
		{
			m_sentinelAddresses = sentinelAddresses;
		}
		void SetMasterName(const string &masterName)
		{
			CSentinelServersConfig::m_sMasterName = masterName;
		}
		void SetDatabase(int database)
		{
			CSentinelServersConfig::m_iDatabase = database;
		}
		void SetScanInterval(int scanInterval)
		{
			CSentinelServersConfig::m_iScanInterval = scanInterval;
		}
	private:
		std::list<string> m_sentinelAddresses;
		string m_sMasterName;
		int m_iDatabase;
		int m_iScanInterval;
	};
}
#endif //EXHIREDIS_SENTINEL_SERVERS_CONFIG_HPP
