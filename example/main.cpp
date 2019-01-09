//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include <tuple>
#include <vector>
#include "exhiredis/rstl/rmap.h"
#include "exhiredis/rstl/rscript.h"
#include "exhiredis/redis_clients.h"

using namespace std;
using namespace exhiredis;

template<class T = int>
class Children
{
public:

    T
    Value()
    {
        return value;
    }
private:
    T value;
};

class Test: public std::enable_shared_from_this<Test>
{
public:
    Test()
    {
    }
    shared_ptr<Test> getPtr()
    {
        return shared_from_this();
    }
};

class Test1: public std::enable_shared_from_this<Test1>
{
public:
    Test1()
    {

    }

    ~Test1()
    {
        printf("~Test1\n");
    }

    Test1(const Test1 &test)
    {
        printf("Test1 copy\n");
    }

    Test1(Test1 &&test)
    {
        printf("Test1 move\n");
    }

    Test1 &operator=(const Test1 &test)
    {
        printf("Test1 operator=\n");
    }

    shared_ptr<Test1> getPtr()
    {
        return shared_from_this();
    }
};

class Test2: public Test
{

};

void
testStr()
{
    char *str = "2\0e";
    string string1 = str;
    string string2;
    string2.assign(str, 3);
    printf("name1 = %s,len = %d\n", string1.c_str(), string1.length());
    printf("name2 = %s,len = %d\n", string2.c_str(), string2.length());
    const char *str1 = string2.c_str();
    int i = 0;
    printf("=========================\n");
    while (i < string2.length()) {
        printf("%c", str1[i]);
        i++;
    }
    printf("\n");
}

weak_ptr<Test1> test(shared_ptr<Test1> shared_ptr1)
{
    shared_ptr<Test1> test = shared_ptr1->shared_from_this();
    return weak_ptr<Test1>(test);
}

Test1 aaa(const Test1 &test)
{
    printf("dddddd\n");
    return std::move(test);
}

int
main()
{
    list<int> list1(5);
    shared_ptr<CSingleServerConfig> config = make_shared<CSingleServerConfig>("127.0.0.1", 6379, "");
    config->SetConnectionPoolSize(5);
    shared_ptr<CRedisConfig> redisConf = make_shared<CRedisConfig>(nullptr, config, nullptr);
    shared_ptr<CRedisClients> redisClients = CRedisClients::CreateInstance(redisConf);
    shared_ptr<RMap<RInt, RInt>> map = redisClients->GetMap<RInt, RInt>("TestMap");
    map->Exists(RInt(111));
    auto keys = map->Keys();
    return 0;
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
}