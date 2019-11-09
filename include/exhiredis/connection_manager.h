//
// Created by dguco on 18-11-7.
// redis master slave manager
//

#ifndef EXHIREDIS_MASTERSLAVEMANAGER_HPP
#define EXHIREDIS_MASTERSLAVEMANAGER_HPP

#include <memory>
#include "connection_pool.h"
#include "redis_exception.h"

using namespace std;

namespace exhiredis
{
class CConnectionManager
{
public:
    //构造函数
    CConnectionManager();
    /**
     * init connect manager
     */
    void Init(const string &host, const string &passwd,int poolSize);
    /**
     * add one connection
     * @param
     */
    void PutOneCon(shared_ptr<CRedisConnection> conn);
    /**
     * borrow one connection
     * @return
     */
    shared_ptr<CRedisConnection> GetOneCon();

    /**
     * Execute redis command
     * @tparam Ret return type
     * @param func
     * @return
     */
    template<typename Ret>
    Ret ExecuteCommand(std::function<Ret(shared_ptr<CRedisConnection>)> func)
    {
        shared_ptr<CRedisConnection> tmpConn = GetOneCon();
        try
        {
            return func(tmpConn);
        }
        catch (CRedisException msg)
        {
            PutOneCon(tmpConn);
            throw CRedisException(msg.what());
        }
    }

    /**
     * AsyncExecute redis command
     * @tparam Ret return type
     * @param func
     * @return
     */
    template<typename Ret>
    std::future<Ret> AsyncExecuteCommand(std::function<Ret(shared_ptr<CRedisConnection>)> func)
    {
        return std::async([func] () -> Ret {
            shared_ptr<CRedisConnection> tmpConn = GetOneCon();
            try
            {
                return func(tmpConn);
            }
            catch (CRedisException msg)
            {
                PutOneCon(tmpConn);
                throw CRedisException(msg.what());
            }
        });
    }


    /**
     * Execute redis command
     * @tparam Ret return type
     * @param func
     * @return
     */
    template<typename Ret>
    Ret ExecuteCommand(const vector<std::string> &commands)
    {
        shared_ptr<CRedisConnection> tmpConn = GetOneCon();
        try
        {
            shared_ptr<CRedisReply> reply =   tmpConn->SendCommand(commands);
            CParam<Ret> param;
            param.FromString(reply->StrValue());
            PutOneCon(tmpConn);
            return param.value;
        }
        catch (CRedisException msg)
        {
            PutOneCon(tmpConn);
            throw CRedisException(msg.what());
        }
    }

    /**
     * AsyncExecute redis command
     * @tparam Ret return type
     * @param func
     * @return
     */
    template<typename Ret>
    std::future<Ret> AsyncExecuteCommand(const vector<std::string> &commands)
    {
        return std::async([commands] () -> Ret {
            shared_ptr<CRedisConnection> tmpConn = GetOneCon();
            try
            {
                shared_ptr<CRedisReply> reply =   tmpConn->SendCommand(commands);
                CParam<Ret> param;
                param.FromString(reply->StrValue());
                PutOneCon(tmpConn);
                return param.value;
            }
            catch (CRedisException msg)
            {
                PutOneCon(tmpConn);
                throw CRedisException(msg.what());
            }
        });
    }

private:
    shared_ptr<CConnectionPool> m_pConnectionPool;
};
}
#endif //EXHIREDIS_MASTERSLAVEMANAGER_HPP
