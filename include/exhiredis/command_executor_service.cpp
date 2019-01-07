//
// Created by dguco on 19-1-7.
//

#include "command_executor_service.h"

namespace exhiredis
{
CCommandExecutorService::CCommandExecutorService(const shared_ptr<IConnectionManager> &m_pConnectionManager)
    : m_pConnectionManager(m_pConnectionManager)
{}

future<bool> CCommandExecutorService::RedisAsyncIsSucceedCommand(const char *format,
                                                                 vector<shared_ptr<CCmdParam>> &param,
                                                                 const string &key,
                                                                 eCommandModel model)
{
    shared_ptr<CRedisConnection> conn = GetConn(key, model);
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

future<shared_ptr<long long>> CCommandExecutorService::RedisAsyncReturnIntCommand(const char *format,
                                                                                  vector<shared_ptr<CCmdParam>> &param,
                                                                                  const string &key,
                                                                                  eCommandModel model)
{
    shared_ptr<CRedisConnection> conn = GetConn(key, model);
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
                         return std::move(make_shared<long
                                                      long>(res->integer));
                     }
                     return nullptr;
                 });
}

future<shared_ptr<bool>> CCommandExecutorService::RedisAsyncReturnBoolCommand(const char *format,
                                                                              vector<shared_ptr<CCmdParam>> &param,
                                                                              const string &key,
                                                                              eCommandModel model)
{
    shared_ptr<CRedisConnection> conn = GetConn(key, model);
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
                     else if (res->type == REDIS_REPLY_INTEGER) {
                         if (res->integer == 1) {
                             return std::move(make_shared<bool>(true));
                         }
                         else {
                             return std::move(make_shared<bool>(false));
                         }
                     }
                     else {
                         return nullptr;
                     }
                 });
}

future<shared_ptr<string>> CCommandExecutorService::RedisAsyncReturnStringCommand(const char *format,
                                                                                  vector<shared_ptr<CCmdParam>> &param,
                                                                                  const string &key,
                                                                                  eCommandModel model)
{
    shared_ptr<CRedisConnection> conn = GetConn(key, model);
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

shared_ptr<CRedisConnection> CCommandExecutorService::GetConn(const string &key, eCommandModel model)
{
    int slot = m_pConnectionManager.lock()->CalcSlot(key);
    return m_pConnectionManager.lock()->GetRedisConnection();
}

}
