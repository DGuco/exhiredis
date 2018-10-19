//
// Created by dguco on 18-10-17.
// redis connection
//

#ifndef EXHIREDIS_REDIS_CLIENT_H
#define EXHIREDIS_REDIS_CLIENT_H

#include <stdlib.h>
#include <iostream>
#include <hiredis/async.h>
#include <memory>
#include <uuid.h>
#include <string>
#include <event.h>

using namespace std;

namespace exhiredis
{
	class CRedisConn
	{
	public:
		CRedisConn();
		CRedisConn(const string &sAddress, int iPort);
		virtual ~CRedisConn();
		bool Connect();
	public:
		static void lcb_OnConnectCallback(const redisAsyncContext *c, int status);
		static void lcb_OnDisconnectCallback(const redisAsyncContext *c, int status);
	private:
		redisAsyncContext *m_pRedisContext;
		event_base *m_pEventBase;
		string m_sAddress;
		int m_iPort;
	};
}
#endif //EXHIREDIS_REDIS_CLIENT_H
