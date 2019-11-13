//
// Created by dguco on 18-10-29.
// redis lua 脚本管理
//

#ifndef EXHIREDIS_RSCRIPT_HPP
#define EXHIREDIS_RSCRIPT_HPP

#include <list>
#include <memory>
#include <future>
#include "exhiredis/comman_def.h"
#include "exhiredis/connection_manager.h"

namespace exhiredis
{

    // Rotate an integer value to left.
    inline unsigned int Rol(const unsigned int value,
                              const unsigned int steps);
class RScript
{
public:
    RScript(shared_ptr<CConnectionManager> &pRedisConn);
public:
    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script return int value
     */
    int EvalReturnInt(const string &script,
                      const list<string> &keys,
                      const list<string> &args);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return int value
     */
    future<int> AsyncEvalReturnInt(const string &script,
                                   const list<string> &keys,
                                   const list<string> &args);
    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script return bool value
     */
    bool EvalReturnBool(const string &script,
                        const list<string> &keys,
                        const list<string> &args);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return bool value
     */
    future<bool> AsyncEvalReturnBool(const string &script,
                                     const list<string> &keys,
                                     const list<string> &args);



    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return string value
     */
    string EvalReturnString(const string &script,
                            const list<string> &keys,
                            const list<string> &args);


    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return
     */
    future<string> AsyncEvalReturnString(const string &script,
                                         const list<string> &keys,
                                         const list<string> &args);

    /**
      *
      * @param script
      * @param keys
      * @param args
      * @return lua script return int value
      */
    int EvalshaReturnInt(const string &script,
                         const list<string> &keys,
                         const list<string> &args);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return int value
     */
    future<int> AsyncEvalshaReturnInt(const string &script,
                                      const list<string> &keys,
                                      const list<string> &args);
    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script return bool value
     */
    bool EvalshaReturnBool(const string &script,
                           const list<string> &keys,
                           const list<string> &args);

    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return bool value
     */
    future<bool> AsyncEvalshaReturnBool(const string &script,
                                        const list<string> &keys,
                                        const list<string> &args);
    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return lua script async return string value
     */
    string EvalshaReturnString(const string &script,
                                const list<string> &keys,
                                const list<string> &args);


    /**
     *
     * @param script
     * @param keys
     * @param args
     * @return
     */
    future<string> AsyncEvalshaReturnString(const string &script,
                                             const list<string> &keys,
                                             const list<string> &args);
    /**
     *
     * @tparam return_type
     * @param scriptCmd
     * @param keys
     * @param args
     * @return lua script return return_type value
     */
    template<typename return_type>
    return_type EvalScript(const string &scriptCmd,
                           const list<string> &keys,
                           const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param scriptCmd
     * @param keys
     * @param args
     * @return lua script async return return_type value
     */
    template<typename return_type>
    future<return_type> AsyncEvalScript(const string &scriptCmd,
                                        const list<string> &keys,
                                        const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param script
     * @param keys
     * @param args
     * @return lua script file return return_type value
     */
    template<typename return_type>
    return_type EvalScriptFile(const string &path,
                               const list<string> &keys,
                               const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param script
     * @param keys
     * @param args
     * @return lua script file async return return_type value
     */
    template<typename return_type>
    future<return_type> AsyncEvalScriptFile(const string &path,
                                            const list<string> &keys,
                                            const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param scriptCmd
     * @param keys
     * @param args
     * @return lua script return return_type value
     */
    template<typename return_type>
    return_type EvalshaScript(const string &scriptCmd,
                           const list<string> &keys,
                           const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param scriptCmd
     * @param keys
     * @param args
     * @return lua script async return return_type value
     */
    template<typename return_type>
    future<return_type> AsyncEvalshaScript(const string &scriptCmd,
                                            const list<string> &keys,
                                            const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param script
     * @param keys
     * @param args
     * @return lua script file return return_type value
     */
    template<typename return_type>
    return_type EvalshaScriptFile(const string &path,
                               const list<string> &keys,
                               const list<string> &args);

    /**
     *
     * @tparam return_type
     * @param script
     * @param keys
     * @param args
     * @return lua script file async return return_type value
     */
    template<typename return_type>
    future<return_type> AsyncEvalshaScriptFile(const string &path,
                                            const list<string> &keys,
                                            const list<string> &args);

private:
    /**
     * build script commands
     * @param keys
     * @param args
     * @return
     */
    vector<string> BuildScriptCmd(const string redisCmd,
                                  const string scriptArg,
                                  const list<string> &keys,
                                  const list<string> &args);
    /**
     * read file
     * @param path
     * @return
     */
    string ReadFile(const string&path);

    void ToHexString(const unsigned char *hash, char *hexstring);

    void Calc(const void *src, const int bytelength, unsigned char *hash);

    void InnerHash(unsigned int *result, unsigned int *w);

    void ClearWBuffert(unsigned int *buffert);
private:
    shared_ptr<CConnectionManager> m_pConnectionManager;
};
}
#endif //EXHIREDIS_RSCRIPT_HPP
