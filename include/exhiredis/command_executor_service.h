//
// Created by dguco on 19-1-3.
//

#ifndef EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP
#define EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP

#include <future>
#include <memory>
#include <vector>
#include "comman_def.h"
#include "connection_manager.h"

using namespace std;
namespace exhiredis
{
class CCmdParam;
class CRedisConnection;
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
                                                      eCommandModel model = eCommandModel::NOT_READ_ONLY);
    //execute redis command return string list pointer or nullptr
    future<shared_ptr<list<string>>>
    RedisAsyncReturnStringListCommand(const char *format,
                                      vector<shared_ptr<CCmdParam>> &param,
                                      const string &key = "",
                                      eCommandModel model = eCommandModel::NOT_READ_ONLY);
    //execute redis command return array pointer or nullptr
    template<class return_type>
    future<shared_ptr<list<return_type>>>
    RedisAsyncReturnListCommand(const char *format,
                                vector<shared_ptr<CCmdParam>> &param,
                                const string &key = "",
                                eCommandModel model = eCommandModel::NOT_READ_ONLY);
    
    template<class left_type, class right_type>
    future<shared_ptr<list<pair<left_type, right_type>>>>
    RedisAsyncReturnPairListCommand(const char *format,
                                    vector<shared_ptr<CCmdParam>> &param,
                                    const string &key = "",
                                    eCommandModel model = eCommandModel::NOT_READ_ONLY);

private:
    shared_ptr<CRedisConnection> GetConn(const string &key, eCommandModel model);
private:
    weak_ptr<IConnectionManager> m_pConnectionManager;
};

}
#endif //EXHIREDIS_COMMAND_ASYNC_SERVICE_HPP
