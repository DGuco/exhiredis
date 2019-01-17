//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include <tuple>
#include <vector>
#include <list>
#include <ucontext.h>
#include "exhiredis/rstl/rmap.h"
#include "exhiredis/rstl/rscript.h"
#include "exhiredis/redis_clients.h"
#include "exhiredis/utils/check.h"

using namespace std;
using namespace exhiredis;

void testHiredis()
{
    list<int> list1(5);
    shared_ptr<CSingleServerConfig> config = make_shared<CSingleServerConfig>("127.0.0.1", 6379, "");
    config->SetConnectionPoolSize(5);
    shared_ptr<CRedisConfig> redisConf = make_shared<CRedisConfig>(nullptr, config, nullptr);
    shared_ptr<CRedisClients> redisClients = CRedisClients::CreateInstance(redisConf);
    shared_ptr<RMap<RInt, RInt>> map = redisClients->GetMap<RInt, RInt>("TestMap");
    map->Exists(RInt(111));
    auto keys = map->Keys();
//    RScript *rScript = new RScript(conn);
//    string cmd = "if (redis.call('exists', KEYS[1]) == 0) then " \
//        "redis.call('hset', KEYS[1], ARGV[2], 1); " \
//        "redis.call('pexpire', KEYS[1], ARGV[1]); " \
//        "return nil; " \
//        "end; " \
//        "if (redis.call('hexists', KEYS[1], ARGV[2]) == 1) then " \
//        "redis.call('hincrby', KEYS[1], ARGV[2], 1); " \
//        "redis.call('pexpire', KEYS[1], ARGV[1]); " \
//        "return nil; " \
//        "end; " \
//        "return redis.call('pttl', KEYS[1]);";
//    shared_ptr<long long> res = rScript->EvalReturnInt(cmd, {"TestLock:lock1"}, {"1000000000000", "3133213232"});
};

int
main()
{
    return 0;
}