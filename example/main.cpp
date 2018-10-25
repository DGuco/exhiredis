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

int main()
{
	Children<> *children = new Children<>( );
	int a = children->Value( );
	RMap<int, int> *map = new RMap<int, int>( );
	int a1 = 1;
	int b1 = 2;
	map->Put(a1, b1);
}