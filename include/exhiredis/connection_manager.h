//
// Created by dguco on 18-11-7.
// redis master slave manager
//

#ifndef EXHIREDIS_MASTERSLAVEMANAGER_HPP
#define EXHIREDIS_MASTERSLAVEMANAGER_HPP

#include <memory>
#include <future>
#include <list>
#include "exhiredis/rstl/param.h"
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
    void Init(const string &host,int port, const string &passwd,int poolSize);
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
    Ret ExecuteCommand(const vector<std::string> &commands,
                       std::function<Ret(shared_ptr<CRedisReply>)> func)
    {
        shared_ptr<CRedisConnection> tmpConn = GetOneCon();
        try
        {
            shared_ptr<CRedisReply> reply = tmpConn->SendCommand(commands);
            auto res =  func(reply);
            PutOneCon(tmpConn);
            return std::move(res);
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
    future<Ret> AsyncExecuteCommand(const vector<std::string> &commands,
                                         std::function<Ret(shared_ptr<CRedisReply>)> func)
    {
        return std::async([this,commands,func] () -> Ret {
            shared_ptr<CRedisConnection> tmpConn = this->GetOneCon();
            try
            {
                shared_ptr<CRedisReply> reply = tmpConn->SendCommand(commands);
                auto res = func(reply);
                this->PutOneCon(tmpConn);
                return  std::move(res);
            }
            catch (CRedisException msg)
            {
                this->PutOneCon(tmpConn);
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
        return std::async([this,commands] () -> Ret {
            shared_ptr<CRedisConnection> tmpConn = this->GetOneCon();
            try
            {
                shared_ptr<CRedisReply> reply =   tmpConn->SendCommand(commands);
                CParam<Ret> param;
                param.FromString(reply->StrValue());
                this->PutOneCon(tmpConn);
                return param.value;
            }
            catch (CRedisException msg)
            {
                this->PutOneCon(tmpConn);
                throw CRedisException(msg.what());
            }
        });
    }

    /**
     * Execute redis command
     * @tparam Ret return type list
     * @param func
     * @return
     */
    template<typename Ret>
    list<Ret> ExecuteCommandReturnList(const vector<std::string> &commands)
    {
        shared_ptr<CRedisConnection> tmpConn = GetOneCon();
        try
        {

            shared_ptr<CRedisReply> reply = tmpConn->SendCommand(commands);
            if (reply->ReplyType() != CRedisReply::eReplyType::ARRAY)
            {
                throw CRedisException("Redis reply type is not array");
            }
            list<Ret> resList;
            resList.resize(reply->ArrayElements().size());
            for(const CRedisReply& it : reply->ArrayElements())
            {
                CParam<Ret> param;
                param.FromString(it.StrValue());
                resList.push_back(param.value);
            }
            PutOneCon(tmpConn);
            return std::move(resList);
        }
        catch (CRedisException msg)
        {
            PutOneCon(tmpConn);
            throw CRedisException(msg.what());
        }
    }

    /**
     * AsyncExecute redis command
     * @tparam Ret return type list
     * @param func
     * @return
     */
    template<typename Ret>
    future<list<Ret>> AsyncExecuteCommandReturnList(const vector<std::string> &commands)
    {
        return std::async([this,commands] () -> list<Ret> {
            shared_ptr<CRedisConnection> tmpConn = this->GetOneCon();
            try
            {
                shared_ptr<CRedisReply> reply = tmpConn->SendCommand(commands);
                if (reply->ReplyType() != CRedisReply::eReplyType::ARRAY)
                {
                    throw CRedisException("Redis reply type is not array");
                }
                list<Ret> resList;
                resList.resize(reply->ArrayElements().size());
                for(const CRedisReply& it : reply->ArrayElements())
                {
                    CParam<Ret> param;
                    param.FromString(it.StrValue());
                    resList.push_back(param.value);
                }
                CParam<Ret> param;
                param.FromString(reply->StrValue());
                this->PutOneCon(tmpConn);
                return std::move(resList);
            }
            catch (CRedisException msg)
            {
                this->PutOneCon(tmpConn);
                throw CRedisException(msg.what());
            }
        });
    }


    /**
     * Execute redis command
     * @tparam Ret return type list
     * @param func
     * @return
     */
    template<typename Ret>
    list<Ret> ExecuteCommandReturnList(const vector<std::string> &commands,
                                       std::function<list<Ret>(shared_ptr<CRedisReply>)> func)
    {
        shared_ptr<CRedisConnection> tmpConn = GetOneCon();
        try
        {
            shared_ptr<CRedisReply> redisReply = tmpConn->SendCommand(commands);
            auto res = func(redisReply);
            PutOneCon(tmpConn);
            return std::move(res);
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
    std::future<list<Ret>> AsyncExecuteCommandReturnList(const vector<std::string> &commands,
                                                         std::function<list<Ret>(shared_ptr<CRedisReply>)> func)
    {
        return std::async([this,commands,func] () -> list<Ret> {
            shared_ptr<CRedisConnection> tmpConn = this->GetOneCon();
            try
            {
                shared_ptr<CRedisReply> redisReply = tmpConn->SendCommand(commands);
                auto res = func(redisReply);
                this->PutOneCon(tmpConn);
                return std::move(res);
            }
            catch (CRedisException msg)
            {
                this->PutOneCon(tmpConn);
                throw CRedisException(msg.what());
            }
        });
    }

private:
    shared_ptr<CConnectionPool> m_pConnectionPool;
};
}
#endif //EXHIREDIS_MASTERSLAVEMANAGER_HPP
