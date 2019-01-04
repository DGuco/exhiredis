//
// Created by dguco on 19-1-3.
//

#include "rscript.h"
#include "exhiredis/redis_clients.h"

namespace exhiredis
{
RScript::RScript(const shared_ptr<CRedisClients> &pRedisConn)
    : m_pRedisClients(pRedisConn)
{

}

shared_ptr<long long> RScript::EvalReturnInt(const string &script,
                                             const list<string> &keys,
                                             const list<string> &args,
                                             eCommandModel model)
{
    return EvalReturnIntAsync(script, keys, args, model).get();
}

future<shared_ptr<long long>> RScript::EvalReturnIntAsync(const string &script,
                                                          const list<string> &keys,
                                                          const list<string> &args,
                                                          eCommandModel model)
{
    vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(script)};
    return m_pRedisClients->GetConnectionManager()->GetCommandExecutorService()
        ->RedisAsyncReturnIntCommand(BuildScriptCmd(keys, args).c_str(), list, "", model);
}

shared_ptr<bool> RScript::EvalReturnBool(const string &script,
                                         const list<string> &keys,
                                         const list<string> &args,
                                         eCommandModel model)
{
    return EvalReturnBoolAsync(script, keys, args, model).get();
}

future<shared_ptr<bool>> RScript::EvalReturnBoolAsync(const string &script,
                                                      const list<string> &keys,
                                                      const list<string> &args,
                                                      eCommandModel model)
{
    vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(script)};
    return m_pRedisClients->GetConnectionManager()->GetCommandExecutorService()
        ->RedisAsyncReturnBoolCommand(BuildScriptCmd(keys, args).c_str(), list, "", model);
}

template<class return_type>
shared_ptr<return_type> RScript::Eval(const string &script,
                                      const list<string> &keys,
                                      const list<string> &args,
                                      eCommandModel model)
{
    return EvalAsync<return_type>(script, keys, args, model).get();

}

template<class return_type>
future<shared_ptr<return_type>> RScript::EvalAsync(const string &script,
                                                   const list<string> &keys,
                                                   const list<string> &args,
                                                   eCommandModel model)
{
    vector<shared_ptr<CCmdParam>> list = {make_shared<CCmdParam>(script)};
    return m_pRedisClients->GetConnectionManager()->GetCommandExecutorService()
        ->RedisAsyncCommand<return_type>(BuildScriptCmd(keys, args).c_str(), list, "", model);
}

const string RScript::BuildScriptCmd(const list<string> &keys, const list<string> &args)
{
    string scriptCmd = CRedisCommands::EVAL + to_string(keys.size()) + " ";
    for (auto str : keys) {
        scriptCmd += (" " + str);
    }
    for (auto str : args) {
        scriptCmd += (" " + str);
    }
    return scriptCmd;
}
}