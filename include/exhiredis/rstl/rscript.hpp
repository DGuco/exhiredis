//
// Created by dguco on 18-10-29.
// redis lua 脚本管理
//

#ifndef EXHIREDIS_RSCRIPT_HPP
#define EXHIREDIS_RSCRIPT_HPP

#include <list>
#include "exhiredis/redis_connection.hpp"
#include "rmap.hpp"

namespace exhiredis
{
class CRedisClients;
class RScript
{
public:
    RScript(const shared_ptr<CRedisClients> &pRedisConn);
public:
    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script return int value or null for failed
     */
    shared_ptr<long long> EvalReturnInt(const string &script,
                                        const list<string> &keys,
                                        const list<string> &args);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return int value or null for failed
     */
    future<shared_ptr<long long>> EvalReturnIntAsync(const string &script,
                                                     const list<string> &keys,
                                                     const list<string> &args);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script return bool value or null for failed
     */
    shared_ptr<bool> EvalReturnBool(const string &script,
                                    const list<string> &keys,
                                    const list<string> &args);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return bool value or null for failed
     */
    future<shared_ptr<bool>> EvalReturnBoolAsync(const string &script,
                                                 const list<string> &keys,
                                                 const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param script
     * @param keys
     * @param args
     * @return lua script return return_type value or null for failed 必须实现IRobject接口
     */
    template<class return_type>
    shared_ptr<return_type> Eval(const string &script,
                                 const list<string> &keys,
                                 const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param script
     * @param keys
     * @param args
     * @return lua script async return return_type value or null for failed 必须实现IRobject接口
     */
    template<class return_type>
    future<shared_ptr<return_type>> EvalAsync(const string &script,
                                              const list<string> &keys,
                                              const list<string> &args);
private:
    const string BuildScriptCmd(const list<string> &keys, const list<string> &args);
private:
    shared_ptr<CRedisClients> m_pRedisClients;
};

RScript::RScript(const shared_ptr<CRedisClients> &pRedisConn)
    : m_pRedisClients(pRedisConn)
{

}

shared_ptr<long long> RScript::EvalReturnInt(const string &script,
                                             const list<string> &keys,
                                             const list<string> &args)
{
    return EvalReturnIntAsync(script, keys, args).get();
}

future<shared_ptr<long long>> RScript::EvalReturnIntAsync(const string &script,
                                                          const list<string> &keys,
                                                          const list<string> &args)
{
    vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(script)};
    return m_pRedisClients->GetConnectionManager()->GetCommandExecutorService()
    RedisAsyncReturnIntCommand(m_pRedisClients, BuildScriptCmd(keys, args).c_str(), list);
}

shared_ptr<bool> RScript::EvalReturnBool(const string &script,
                                         const list<string> &keys,
                                         const list<string> &args)
{
    return EvalReturnBoolAsync(script, keys, args).get();
}

future<shared_ptr<bool>> RScript::EvalReturnBoolAsync(const string &script,
                                                      const list<string> &keys,
                                                      const list<string> &args)
{
    vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(script)};
    return CCommandExecutorService::RedisAsyncReturnBoolCommand(m_pRedisClients,
                                                                BuildScriptCmd(keys, args).c_str(),
                                                                list);
}

template<class return_type>
shared_ptr<return_type> RScript::Eval(const string &script,
                                      const list<string> &keys,
                                      const list<string> &args)
{
    return EvalAsync<return_type>(script, keys, args).get();

}

template<class return_type>
future<shared_ptr<return_type>> RScript::EvalAsync(const string &script,
                                                   const list<string> &keys,
                                                   const list<string> &args)
{
    vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(script)};
    return CCommandExecutorService::RedisAsyncCommand<return_type>(m_pRedisClients,
                                                                   BuildScriptCmd(keys, args).c_str(),
                                                                   list);
}

const string RScript::BuildScriptCmd(const list<string> &keys, const list<string> &args)
{
    string scriptCmd = redis_commands::EVAL + to_string(keys.size()) + " ";
    for (auto str : keys) {
        scriptCmd += (" " + str);
    }
    for (auto str : args) {
        scriptCmd += (" " + str);
    }
    return scriptCmd;
}
}
#endif //EXHIREDIS_RSCRIPT_HPP
