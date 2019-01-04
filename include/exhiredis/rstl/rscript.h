//
// Created by dguco on 18-10-29.
// redis lua 脚本管理
//

#ifndef EXHIREDIS_RSCRIPT_HPP
#define EXHIREDIS_RSCRIPT_HPP

#include <list>
#include "exhiredis/redis_connection.h"
#include "rmap.h"
#include "exhiredis/comman_def.h"

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
                                        const list<string> &args,
                                        eCommandModel model);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return int value or null for failed
     */
    future<shared_ptr<long long>> EvalReturnIntAsync(const string &script,
                                                     const list<string> &keys,
                                                     const list<string> &args,
                                                     eCommandModel model);
    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script return bool value or null for failed
     */
    shared_ptr<bool> EvalReturnBool(const string &script,
                                    const list<string> &keys,
                                    const list<string> &args,
                                    eCommandModel model);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return bool value or null for failed
     */
    future<shared_ptr<bool>> EvalReturnBoolAsync(const string &script,
                                                 const list<string> &keys,
                                                 const list<string> &args,
                                                 eCommandModel model);

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
                                 const list<string> &args,
                                 eCommandModel model);

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
                                              const list<string> &args,
                                              eCommandModel model);
private:
    const string BuildScriptCmd(const list<string> &keys, const list<string> &args);
private:
    shared_ptr<CRedisClients> m_pRedisClients;
};
}
#endif //EXHIREDIS_RSCRIPT_HPP
