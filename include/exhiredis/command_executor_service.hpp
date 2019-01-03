//
// Created by dguco on 19-1-3.
//

#ifndef EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP
#define EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP

#include <future>
#include <memory>
#include "exhiredis/redis_connection.hpp"
#include "exhiredis/command_param.hpp"

namespace exhiredis
{
class CCommandExecutorService
{
public:
    //execute redis command ,don't care what redis returns,only care whether the command succeed or not
    static future<bool> RedisAsyncIsSucceedCommand(shared_ptr<CRedisConnection> conn,
                                                   const char *format,
                                                   vector<shared_ptr<CCmdParam>> &param);
    //execute redis command return integer pointer or nullptr
    static future<shared_ptr<long long>> RedisAsyncReturnIntCommand(shared_ptr<CRedisConnection> conn,
                                                                    const char *format,
                                                                    vector<shared_ptr<CCmdParam>> &param);
    //execute redis command return bool pointer or nullptr
    static future<shared_ptr<bool>> RedisAsyncReturnBoolCommand(shared_ptr<CRedisConnection> conn,
                                                                const char *format,
                                                                vector<shared_ptr<CCmdParam>> &param);
    //execute redis command return string pointer or nullptr
    static future<shared_ptr<string>> RedisAsyncReturnStringCommand(shared_ptr<CRedisConnection> conn,
                                                                    const char *format,
                                                                    vector<shared_ptr<CCmdParam>> &param);
    //execute redis command return bool pointer or nullptr,return_type must be the subclass of the IRobject
    template<class return_type>
    static future<shared_ptr<return_type>> RedisAsyncCommand(shared_ptr<CRedisConnection> conn,
                                                             const char *format,
                                                             vector<shared_ptr<CCmdParam>> &param);
    //execute redis command return string list pointer or nullptr
    future<shared_ptr<list<string>>>
    static RedisAsyncReturnStringListCommand(shared_ptr<CRedisConnection> conn,
                                             const char *format,
                                             vector<shared_ptr<CCmdParam>> &param);
    //execute redis command return array pointer or nullptr
    template<class return_type>
    static future<shared_ptr<list<shared_ptr<return_type>>>>
    RedisAsyncReturnListCommand(shared_ptr<CRedisConnection> conn,
                                const char *format,
                                vector<shared_ptr<CCmdParam>> &param);
};

future<bool> CCommandExecutorService::RedisAsyncIsSucceedCommand(shared_ptr<CRedisConnection> conn,
                                                                const char *format,
                                                                vector<shared_ptr<CCmdParam>> &param)
{
    shared_ptr<CCommand> command = conn->RedisvAsyncCommand(format, param);
    return async([command]() -> bool
                 {
                     redisReply *res = command->GetPromise()->get_future().get();
                     if (res == nullptr) {
                         return false;
                     }
                     if (res->type == REDIS_REPLY_ERROR) {
                         HIREDIS_LOG_ERROR("Redis reply error,error msg: %s", res->str);
                         return false;
                     }

                     if (res->type == REDIS_REPLY_STATUS) {
                         return strcmp(res->str, "OK") == 0;
                     }
                     //don't care what the result is
                     return true;
                 });
}

future<shared_ptr<long long>> CCommandExecutorService::RedisAsyncReturnIntCommand(shared_ptr<CRedisConnection> conn,
                                                                                 const char *format,
                                                                                 vector<shared_ptr<CCmdParam>> &param)
{
    shared_ptr<CCommand> command = conn->RedisvAsyncCommand(format, param);
    return async([command]() -> shared_ptr<long long>
                 {
                     redisReply *res = command->GetPromise()->get_future().get();
                     if (res == nullptr) {
                         return nullptr;
                     }
                     if (res->type == REDIS_REPLY_ERROR) {
                         HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                         return nullptr;
                     }

                     if (res->type == REDIS_REPLY_INTEGER) {
                         return std::move(make_shared<long long>(res->integer));
                     }
                     return nullptr;
                 });
}

future<shared_ptr<bool>> CCommandExecutorService::RedisAsyncReturnBoolCommand(shared_ptr<CRedisConnection> conn,
                                                                             const char *format,
                                                                             vector<shared_ptr<CCmdParam>> &param)
{
    shared_ptr<CCommand> command = conn->RedisvAsyncCommand(format, param);
    return async([command]() -> shared_ptr<bool>
                 {
                     redisReply *res = command->GetPromise()->get_future().get();
                     if (res == nullptr) {
                         return nullptr;
                     }
                     if (res->type == REDIS_REPLY_ERROR) {
                         HIREDIS_LOG_ERROR("Redis reply error,error msg: %s.", res->str);
                         return nullptr;
                     }

                     /*
                        *Lua boolean false -> Redis Nil bulk reply / Lua 的布尔值 false 转换成 Redis 的 Nil bulk 回复
                        *Lua boolean true -> Redis integer reply with value of 1 / Lua 布尔值 true 转换成 Redis 整数回复中的 1
                       **/
                     if (res->type == REDIS_REPLY_NIL) {
                         return std::move(make_shared<bool>(false));
                     }
                     else if (res->type == REDIS_REPLY_INTEGER && res->integer == 1) {
                         return std::move(make_shared<bool>(true));
                     }
                     else {
                         return nullptr;
                     }
                 });
}

future<shared_ptr<string>> CCommandExecutorService::RedisAsyncReturnStringCommand(shared_ptr<CRedisConnection> conn,
                                                                                 const char *format,
                                                                                 vector<shared_ptr<CCmdParam>> &param)
{
    shared_ptr<CCommand> tmpCommand = conn->RedisvAsyncCommand(format, param);
    return async([tmpCommand]() -> shared_ptr<string>
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

                     if (res->type == REDIS_REPLY_INTEGER) {
                         string resValue;
                         shared_ptr<string> value = make_shared<string>(to_string(res->integer));
                         return value;
                     }

                     if (res->type == REDIS_REPLY_STRING) {
                         string resValue;
                         //binary safe
                         resValue.assign(res->str, res->len);
                         //resValue = res->str; error:not binary safe
                         shared_ptr<string> value = make_shared<string>(resValue);
                         return std::move(value);
                     }
                     return nullptr;
                 });
}

future<shared_ptr<list<string>>> CCommandExecutorService::RedisAsyncReturnStringListCommand(shared_ptr<CRedisConnection> conn,
                                                                                           const char *format,
                                                                                           vector<shared_ptr<CCmdParam>> &param)
{
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
                     shared_ptr<list<string>> res_list = make_shared<list<string>>(res->elements);
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

template<class return_type>
future<shared_ptr<list<shared_ptr<return_type>>>>
CCommandExecutorService::RedisAsyncReturnListCommand(shared_ptr<CRedisConnection> conn,
                                                    const char *format,
                                                    vector<shared_ptr<CCmdParam>> &param)
{
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
                     typedef list<shared_ptr<return_type>> res_type;
                     shared_ptr<res_type> res_list = make_shared<res_type>(res->elements);
                     for (size_t i = 0; i < res->elements; i++) {
                         redisReply *reply = res->element[i];
                         shared_ptr<return_type> value = make_shared<return_type>();
                         string resValue;
                         //binary safe 二进制安全
                         resValue.assign(reply->str, reply->len);
                         //resValue = res->str; error:not binary safe 非二进制安全
                         static_pointer_cast<IRobject>(value)->FromString(resValue);
                         res_list->push_back(std::move(value));
                     }
                     return std::move(res_list);
                 });
}

template<class return_type>
future<shared_ptr<return_type>> CCommandExecutorService::RedisAsyncCommand(shared_ptr<CRedisConnection> conn,
                                                                          const char *format,
                                                                          vector<shared_ptr<CCmdParam>> &param)
{
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

}
#endif //EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP
