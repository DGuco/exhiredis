//
// Created by dguco on 19-10-19.
//

#ifndef EXHIREDIS_REDIS_CONNECTION_H
#define EXHIREDIS_REDIS_CONNECTION_H

#endif //EXHIREDIS_REDIS_CONNECTION_H

#include <string>
#include "hiredis/hiredis.h"

using namespace std;

namespace  exhiredis
{
    enum class eConnRole
    {
        ANY = 0,
        MASTER = 1,
        SLAVE = 2,
    };

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
    private:
        redisContext* m_context;
        string m_sAddressName;
        string m_sHost;
        int m_iPort;
        int m_iTimeOut;
    };
}
