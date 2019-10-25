//
// Created by dguco on 19-1-3.
//

#ifndef EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP
#define EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP

#include <future>
#include <memory>
#include <vector>
#include "exhiredis/robject/robject.h"
#include "comman_def.h"
#include "connection_manager.h"
#include "command_param.h"
#include "command.h"
#include "redis_async_connection.h"

using namespace std;
namespace exhiredis
{
class CCommandExecutorService
{
public:
    CCommandExecutorService(const shared_ptr<IConnectionManager> &m_pConnectionManager);
    //execute redis command ,don't care what redis returns,only care whether the command succeed or not
    future<bool> RedisAsyncIsSucceedCommand(const char *format,
                                            vector<shared_ptr<CCmdParam>> &param,
                                            const string &key = "",
                                            eCommandModel model = eCommandModel::NOT_READ_ONLY);
    //execute redis command return integer pointer or nullptr
    future<shared_ptr<long long>> RedisAsyncReturnIntCommand(const char *format,
                                                             vector<shared_ptr<CCmdParam>> &param,
                                                             const string &key = "",
                                                             eCommandModel model = eCommandModel::NOT_READ_ONLY);
    //execute redis command return bool pointer or nullptr
    future<shared_ptr<bool>> RedisAsyncReturnBoolCommand(const char *format,
                                                         vector<shared_ptr<CCmdParam>> &param,
                                                         const string &key = "",
                                                         eCommandModel model = eCommandModel::NOT_READ_ONLY);
    //execute redis command return string pointer or nullptr
    future<shared_ptr<string>> RedisAsyncReturnStringCommand(const char *format,
                                                             vector<shared_ptr<CCmdParam>> &param,
                                                             const string &key = "",
                                                             eCommandModel model = eCommandModel::NOT_READ_ONLY);
    //execute redis command return bool pointer or nullptr,return_type must be the subclass of the IRobject
    template<class return_type>
    future<shared_ptr<return_type>> RedisAsyncCommand(const char *format,
                                                      vector<shared_ptr<CCmdParam>> &param,
                                                      const string &key = "",
                                                      eCommandModel model = eCommandModel::NOT_READ_ONLY)
    {
        shared_ptr<CRedisAsyncConnection> conn = GetConn(key, model);
        shared_ptr<CCommand> tmpCommand = conn->RedisvAsyncCommand(format, param);
        return async([tmpCommand]() -> shared_ptr<return_type>
                     {
                         redisReply *res = tmpCommand->GetPromise()->get_future().get();
                         if (res == nullptr) {
                             return nullptr;
                         }

                         if (res->type == REDIS_REPLY_ERROR) {
                             HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                             return nullptr;
                         }

                         if (res->type == REDIS_REPLY_NIL) {
                             return nullptr;
                         }

                         if (res->type == REDIS_REPLY_STRING) {
                             shared_ptr<return_type> value = make_shared<return_type>();
                             string resValue;
                             //binary safe 二进制安全
                             resValue.assign(res->str, res->len);
                             //resValue = res->str; error:not binary safe 非二进制安全
                             static_pointer_cast<IRobject>(value)->FromString(resValue);
                             return std::move(value);
                         }
                         return nullptr;
                     });
    }
    //execute redis command return string list pointer or nullptr
    future<shared_ptr<list<string>>>
    RedisAsyncReturnStringListCommand(const char *format,
                                      vector<shared_ptr<CCmdParam>> &param,
                                      const string &key = "",
                                      eCommandModel model = eCommandModel::NOT_READ_ONLY)
    {
        shared_ptr<CRedisAsyncConnection> conn = GetConn(key, model);
        shared_ptr<CCommand> tmpCommand = conn->RedisvAsyncCommand(format, param);
        return async([tmpCommand]() -> shared_ptr<list<string>>
                     {
                         future<redisReply *> future = tmpCommand->GetPromise()->get_future();
                         redisReply *res = future.get();
                         if (res == nullptr) {
                             return nullptr;
                         }

                         if (res->type == REDIS_REPLY_ERROR) {
                             HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                             return nullptr;
                         }

                         if (res->type != REDIS_REPLY_ARRAY) {
                             HIREDIS_LOG_ERROR("Redis reply is not array type,");
                             return nullptr;
                         }
                         shared_ptr<list<string>> res_list = make_shared<list<string >>(res->elements);
                         for (size_t i = 0; i < res->elements; i++) {
                             redisReply *reply = res->element[i];
                             string resValue;
                             //binary safe 二进制安全
                             resValue.assign(reply->str, reply->len);
                             //resValue = res->str; error:not binary safe 非二进制安全
                             res_list->push_back(resValue);
                         }
                         return std::move(res_list);
                     });
    }

    //execute redis command return array pointer or nullptr
    template<class return_type>
    future<shared_ptr<list<return_type>>>
    RedisAsyncReturnListCommand(const char *format,
                                vector<shared_ptr<CCmdParam>> &param,
                                const string &key = "",
                                eCommandModel model = eCommandModel::NOT_READ_ONLY)
    {
        shared_ptr<CRedisAsyncConnection> conn = GetConn(key, model);
        shared_ptr<CCommand> tmpCommand = conn->RedisvAsyncCommand(format, param);
        return async([tmpCommand]() -> shared_ptr<list<return_type>>
                     {
                         future<redisReply *> future = tmpCommand->GetPromise()->get_future();
                         redisReply *res = future.get();
                         if (res == nullptr) {
                             return nullptr;
                         }

                         if (res->type == REDIS_REPLY_ERROR) {
                             HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                             return nullptr;
                         }

                         if (res->type != REDIS_REPLY_ARRAY) {
                             HIREDIS_LOG_ERROR("Redis reply is not array type,");
                             return nullptr;
                         }
                         typedef list<return_type> res_type;
                         shared_ptr<res_type> res_list = make_shared<res_type>(res->elements);
                         for (size_t i = 0; i < res->elements; i++) {
                             redisReply *reply = res->element[i];
                             return_type value;
                             string resValue;
                             //binary safe 二进制安全
                             resValue.assign(reply->str, reply->len);
                             //resValue = res->str; error:not binary safe 非二进制安全
                             ((IRobject *) (&value))->FromString(resValue);
                             res_list->push_back(std::move(value));
                         }
                         return std::move(res_list);
                     });
    }

    template<class left_type, class right_type>
    future<shared_ptr<list<pair<left_type, right_type>>>>
    RedisAsyncReturnPairListCommand(const char *format,
                                    vector<shared_ptr<CCmdParam>> &param,
                                    const string &key = "",
                                    eCommandModel model = eCommandModel::NOT_READ_ONLY)
    {
        shared_ptr<CRedisAsyncConnection> conn = GetConn(key, model);
        shared_ptr<CCommand> tmpCommand = conn->RedisvAsyncCommand(format, param);
        return async([tmpCommand]() -> shared_ptr<list<pair<left_type, right_type>>>
                     {
                         future<redisReply *> future = tmpCommand->GetPromise()->get_future();
                         redisReply *res = future.get();
                         if (res == nullptr) {
                             return nullptr;
                         }

                         if (res->type == REDIS_REPLY_ERROR) {
                             HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                             return nullptr;
                         }

                         if (res->type != REDIS_REPLY_ARRAY) {
                             HIREDIS_LOG_ERROR("Redis reply is not array type,");
                             return nullptr;
                         }
                         if (res->elements % 2 != 0) {
                             HIREDIS_LOG_ERROR("Redis reply elements is illegal");
                             return nullptr;
                         }
                         typedef list<pair<left_type, right_type>> res_type;
                         shared_ptr<res_type> res_list = make_shared<res_type>(res->elements);
                         for (size_t i = 0; i < res->elements; i += 2) {
                             redisReply *reply_left = res->element[i];
                             redisReply *reply_right = res->element[i + 1];
                             left_type left_value;
                             left_type right_value;
                             string left_str;
                             string right_str;
                             //binary safe 二进制安全
                             left_str.assign(reply_left->str, reply_left->len);
                             right_str.assign(reply_right->str, reply_right->len);
                             ((IRobject *) (&left_value))->FromString(left_str);
                             ((IRobject *) (&right_value))->FromString(right_str);
                             res_list->push_back(move(make_pair<left_type, right_type>(move(left_value),
                                                                                       move(right_value))));
                         }
                         return std::move(res_list);
                     });
    };

private:
    shared_ptr<CRedisAsyncConnection> GetConn(const string &key, eCommandModel model);
private:
    weak_ptr<IConnectionManager> m_pConnectionManager;
};

}
#endif //EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP
