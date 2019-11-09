//
// Created by dguco on 18-11-7.
// redis master slave manager
//

#ifndef EXHIREDIS_MASTERSLAVEMANAGER_HPP
#define EXHIREDIS_MASTERSLAVEMANAGER_HPP

#include <memory>
#include "connection_pool.h"

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
private:
    shared_ptr<CConnectionPool> m_pConnectionPool;
};
}
#endif //EXHIREDIS_MASTERSLAVEMANAGER_HPP
