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

    shared_ptr<CRedisClients> redisClients = CRedisClients::CreateInstance("127.0.0.1",6379,"",5);
    RMap<int, int> map = redisClients->GetMap<int, int>("TestMap");
    bool res = map.Exists(111);
    auto keys = map.Keys();
    map.GetAll();
    RScript rScript = redisClients->GetScript();
    string cmd = "if (redis.call('exists', KEYS[1]) == 0) then " \
        "redis.call('hset', KEYS[1], ARGV[2], 1); " \
        "redis.call('pexpire', KEYS[1], ARGV[1]); " \
        "return nil; " \
        "end; " \
        "if (redis.call('hexists', KEYS[1], ARGV[2]) == 1) then " \
        "redis.call('hincrby', KEYS[1], ARGV[2], 1); " \
        "redis.call('pexpire', KEYS[1], ARGV[1]); " \
        "return nil; " \
        "end; " \
        "return redis.call('pttl', KEYS[1]);";
    string resi = rScript.EvalReturnString("return 'hello world!'", {}, {});
    printf("%s\n",resi.c_str());
};

int
main()
{
    testHiredis();
    return 0;
}