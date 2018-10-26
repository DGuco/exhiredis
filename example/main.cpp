//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include "exhiredis/rstl/rmap.hpp"

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

int main()
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

	std::future<int> &&f1 = getFuture( );
}