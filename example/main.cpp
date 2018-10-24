//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include <exhiredis/utils/log.h>
#include <exhiredis/robject/rint.h>

using namespace std;
using namespace exhiredis;

class Test
{
	virtual void get() = 0;
};

class Children: public Test
{
	void get() override
	{

	}
};

class Children1: public Test
{
	void get() override
	{

	}
};

int main()
{
	Test *test1 = new Children;
	Test *test2 = new Children1;
	int type1 = typeid(*test1).hash_code( );
	int type2 = typeid(*test2).hash_code( );
}