//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include <tuple>
#include <vector>
#include <list>
#include "../exhiredis/rstl/rmap.h"
#include "../exhiredis/rstl/rscript.h"
#include "../exhiredis/redis_clients.h"

using namespace std;
using namespace exhiredis;

class TestObj
{
public:
    TestObj() {}

    TestObj(int a, int b) : a(a), b(b) {}

    void FromString(const string &str)
    {
        int index = str.find_first_of('_');
        a = std::stoi(str.substr(0,index));
        b = std::stoi(str.substr(index + 1,str.length()));
    }

    const string ToString()
    {
        string str =  to_string(a) + "_" + to_string(b);
        return  str;
    }
    int a;
    int b;
};
void testHiredis()
{

    shared_ptr<CRedisClients> redisClients = CRedisClients::CreateInstance("127.0.0.1",6379,"",5,0);
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
    RMap<int, TestObj> map1 = redisClients->GetMap<int, TestObj>("TestMap1");
    TestObj obj1(10,100);
    map1.Put(1,obj1);
    std::pair<bool,TestObj> resObj = map1.Get(1);
    printf("resObj a = %d,b = %d \n",resObj.second.a,resObj.second.b);

};

int
main()
{
    testHiredis();
    return 0;
}