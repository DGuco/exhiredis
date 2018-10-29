//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include "exhiredis/rstl/rmap.hpp"
#include "exhiredis/redis_conn.hpp"

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
	Test1 test1;
	Test2 test2;
	string name1 = typeid(1).name( );
	string name2 = typeid(1).name( );
	char str1[100] = "\"\\\"213123123\\ \\0eewqeqwe\\\"\"";
	for (int i = 0; i < 100; i++) {
		printf("%c", str1[i]);
	}
	printf("\n");
	char *str = "\"213123123\ \0eewqeqwe\"";
	string string1 = "\"213123123\ \0eewqeqwe\"";
	printf("name1 = %s\n", str);
	printf("name2 = %s,len = %d\n", string1.c_str( ), string1.length( ));
}
int main()
{
	CLog::CreateInstance( );
	std::shared_ptr<CRedisConn> conn = std::make_shared<CRedisConn>( );
	conn->Connect("127.0.0.1", 6379);
	RMap<RInt, RInt> *rMap = new RMap<RInt, RInt>("TestMap", conn);
	const RInt key = RInt(1);
	const RInt value = RInt(10);
	rMap->Put(key, value);
	std::shared_ptr<RInt> res = rMap->Get(key);
	printf("-------------------\n");
	int intva = res->Value();
	printf("res  = %d \n", intva);
	delete rMap;
}