//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include "exhiredis/rstl/rmap.hpp"
#include "exhiredis/rstl/rscript.hpp"

using namespace std;
using namespace exhiredis;

template<class T = int>
class Children
{
public:

	T Value()
	{
		return value;
	}
private:
	T value;
};

class Test
{

};

class Test1: public Test
{

};

class Test2: public Test
{

};

std::future<int> &&getFuture()
{
	return std::async([]
					  { return 0; });
}

void testStr()
{
	char *str = "2\0e";
	string string1 = str;
	string string2;
	string2.assign(str, 3);
	printf("name1 = %s,len = %d\n", string1.c_str( ), string1.length( ));
	printf("name2 = %s,len = %d\n", string2.c_str( ), string2.length( ));
	const char *str1 = string2.c_str( );
	int i = 0;
	printf("=========================\n");
	while (i < string2.length( )) {
		printf("%c", str1[i]);
		i++;
	}
	printf("\n");
}

void testMap()
{
	std::shared_ptr<CRedisConnection> conn = std::make_shared<CRedisConnection>( );
	conn->Connect("127.0.0.1", 6379);
	RMap<RInt, RInt> *rMap = new RMap<RInt, RInt>("TestMap", conn);
	const RInt key = RInt(1);
	const RInt value = RInt(10);
	rMap->Put(key, value);
	std::shared_ptr<RInt> res = rMap->Get(key);
	printf("-------------------\n");
	int intva = res->Value( );
	printf("res  = %d \n", intva);
	delete rMap;
}

int main()
{
//	testStr( );
//	testMap( );
	CLog::CreateInstance( );
	std::shared_ptr<CRedisConnection> conn = std::make_shared<CRedisConnection>( );
	conn->Connect("127.0.0.1", 6379);
	RScript *rScript = new RScript(conn);
	string cmd = "if (redis.call('exists', KEYS[1]) == 0) then " \
        "redis.call('hset', KEYS[1], ARGV[2], 1); " \
        "redis.call('pexpire', KEYS[1], ARGV[1]); " \
        "return -3; " \
        "end; " \
        "if (redis.call('hexists', KEYS[1], ARGV[2]) == 1) then " \
        "redis.call('hincrby', KEYS[1], ARGV[2], 1); " \
        "redis.call('pexpire', KEYS[1], ARGV[1]); " \
        "return -3; " \
        "end; " \
        "return redis.call('pttl', KEYS[1]);";
	rScript->Eval<string>(cmd.c_str( ), {"TestLock:lock1"}, {"100", "3132"});
	delete rScript;
}