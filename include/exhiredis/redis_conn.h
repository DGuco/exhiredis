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
#include <utils/uuid.h>
#include <string>
#include <event.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

namespace exhiredis
{
	class CRedisConn
	{
	public:
		enum class eConnState
		{
			NOT_YET_CONNECTED = 0, // Starting state
			CONNECTED = 1,         // Successfully connected
			DISCONNECTED = 2,      // Successfully disconnected
			CONNECT_ERROR = 3,     // Error connecting
			DISCONNECT_ERROR = 4,  // Disconnected on error
			INIT_ERROR = 5         // Failed to init data structures
		};
	public:
		CRedisConn();
		virtual ~CRedisConn();
		//连接redis
		bool Connect(const string &host, int port);
		//连接redis
		bool ConnectUnix(const string &address);
		//获取连接状态
		eConnState GetConnState();
		//设置连接状态
		void SetConnState(eConnState eConnState);
	public:
		//连接成功回调
		static void lcb_OnConnectCallback(const redisAsyncContext *c, int status);
		//断开连接回调
		static void lcb_OnDisconnectCallback(const redisAsyncContext *c, int status);
	private:
		//初始化hiredis
		void InitHiredis();
		//初始化libevent
		void InitLibevent();
		//运行eventloop
		void RunEventLoop();
	private:
		redisAsyncContext *m_pRedisContext;
		std::shared_ptr<thread> m_pEventLoopThread;
		event_base *m_pEventBase;
		string m_sHost;
		int m_iPort;
		string m_sAddress;
		bool m_bIsRuning;
		eConnState m_eConnState;
		std::mutex m_connectLock;
		std::condition_variable m_connectWaiter;
		std::mutex m_runingLock;
		std::condition_variable m_runingWaiter;
	};
}
#endif //EXHIREDIS_REDIS_CLIENT_H
