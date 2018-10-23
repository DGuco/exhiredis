//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include <exhiredis/utils/log.h>
#include <exhiredis/robject/rint.h>

using namespace std;
using namespace exhiredis;

int main()
{
	RInt rint(10);
	string str = rint.ToString( );
	int value = rint.Value( );
	printf("str = %s\n,value = %d\n", str.c_str( ), value);

}