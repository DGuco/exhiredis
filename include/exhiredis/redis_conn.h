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
#include <unordered_map>
#include "command.h"

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
		bool ConnecToUnix(const string &address);
		//获取连接状态
		eConnState GetConnState();
		//设置连接状态
		void SetConnState(eConnState eConnState);
		//执行命令
		std::shared_ptr<CCommand> &RedisAsyncCommand(char *cmd, ...);
	private:
		//获取redis command
		std::shared_ptr<CCommand> FindCmd(unsigned long cmdId);
		//remove redis command
		std::shared_ptr<CCommand> RemoveCmd(unsigned long cmdId);
	private:
		//连接成功回调
		static void lcb_OnConnectCallback(const redisAsyncContext *context, int status);
		//断开连接回调
		static void lcb_OnDisconnectCallback(const redisAsyncContext *context, int status);
		//redis command callback
		static void lcb_OnCommandCallback(redisAsyncContext *context, void *reply, void *privdata);
	private:
		//初始化hiredis
		bool InitHiredis();
		//初始化libevent
		bool InitLibevent();
		//运行eventloop
		bool RunEventLoop();
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
		std::unordered_map<unsigned long, std::shared_ptr<CCommand>> m_mCmdMap;
	};
}
#endif //EXHIREDIS_REDIS_CLIENT_H
