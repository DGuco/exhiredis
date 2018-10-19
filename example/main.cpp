//
// Created by dguco on 18-10-18.
//

#include <iostream>

using namespace std;

struct Demo
{
	Demo(int a, int b)
		: a(a), b(b)
	{}
	int a;
	int b;
};
class Test
{
private:
public:
	Test(int j_)
		: j(j_)
	{

	}

	int GetJ() const
	{
		return j;
	}
	int GetA() const
	{
		return a;
	}
	const string &GetB() const
	{
		return b;
	}
private:
	int j;
	int a = 100;
	char *b = const_cast<char *>("C++");
	Demo demo = Demo(1, 2);
};
int main()
{
//	Test test1(1000);
//	int j = test1.GetJ( );
//	int a = test1.GetA( );
//	string b = test1.GetB( );

}