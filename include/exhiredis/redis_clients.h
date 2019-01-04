//
// Created by dguco on 18-11-7.
// redis client
//

#ifndef EXHIREDIS_REDIS_CLIENT_HPP
#define EXHIREDIS_REDIS_CLIENT_HPP

#include <memory>
#include <string>
#include <type_traits>
#include "exhiredis/utils/log.h"
#include "exhiredis/connection_manager.h"
#include "exhiredis/single_connection_manager.h"

using namespace std;
namespace exhiredis
{
template<class key_type, class value_type>
class RMap;

class CRedisClients
{
public:
    //construct
    CRedisClients(shared_ptr<IConnectionManager> &m_pConnectionManager);
    //forbidden operation
    CRedisClients(const CRedisClients &) = delete;
    CRedisClients(const CRedisClients &&) = delete;
    CRedisClients &operator=(CRedisClients &) = delete;
public:
    /**
     *
     * @tparam key_type key_type must be the subclass of the IRobject,you can inherit the class Robject ,ex:RBool,RInt,RFloat......
     * @tparam value_type value_type must be the subclass of the IRobject,you can inherit the class Robject ex:RBool,RInt,RFloat......
     * @param   name hashes name
     * @return  redis hashes  key_type value_type (必须实现IRobject接口,你可以实现Robject接口，例如:RBool,RInt,RFloat......)
     */
    template<class key_type, class value_type>
    shared_ptr<RMap<key_type, value_type>> GetMap(const string &name);
public:
    static shared_ptr<CRedisClients> CreateInstance(shared_ptr<CRedisConfig> config);
    shared_ptr<IConnectionManager> &GetConnectionManager();
private:
    shared_ptr<IConnectionManager> m_pConnectionManager;
};
}

#endif //EXHIREDIS_REDIS_CLIENT_HPP
