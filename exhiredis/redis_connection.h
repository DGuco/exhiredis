//
// Created by dguco on 19-10-19.
//

#ifndef EXHIREDIS_REDIS_CONNECTION_H
#define EXHIREDIS_REDIS_CONNECTION_H

#endif //EXHIREDIS_REDIS_CONNECTION_H

#include <string>
#include <vector>
#include <memory>
#include "redis_reply.h"
#include "../hiredis/hiredis.h"

using namespace std;

namespace  exhiredis
{
    class CRedisConnection
    {
    public:
        //构造函数
        CRedisConnection();
        CRedisConnection(const CRedisConnection &tmp) = delete;
        CRedisConnection(CRedisConnection &&tmp) = delete;
        CRedisConnection &operator=(const CRedisConnection &tmp) = delete;
        //析构函数
        virtual ~CRedisConnection();
        /**
         *
         * @param host
         * @param port
         * @param timeout/seceonds
         * @return
         */
        bool ConnectWithTimeout(const string &host, int port, int timeout);
        /**
         *
         * @param host
         * @param port
         * @return
         */
        bool Connect(const string &host, int port);
        /**
         *
         * @param addressName
         * @param timeout
         * @return
         */
        bool ConnectUnixWithTimeout(const string &addressName, int timeout);
        /**
         *
         * @param addressName
         * @return
         */
        bool ConnectUnix(const string &addressName);
        /**
         *
         * @param commands
         * @return
         */
        shared_ptr<CRedisReply> SendCommand(const vector<std::string> &commands);
        /**
         *
         * @param commands
         */
        void AppendCommand(const vector<std::string> &commands);
        /**
         *
         * @return
         */
        shared_ptr<CRedisReply> GetReply();
        /**
         *
         * @param count
         * @return
         */
        vector<shared_ptr<CRedisReply>> GetReplies(int count);
    private:
        redisContext* m_context;
        string m_sAddressName;
        string m_sHost;
        int m_iPort;
        int m_iTimeOut;
    };
}
