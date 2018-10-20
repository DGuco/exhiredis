//
// Created by dguco on 18-10-18.
//

#include <iostream>
#include <exhiredis/utils/log.h>

using namespace std;
using namespace exhiredis;

int main()
{
//	Test test1(1000);
//	int j = test1.GetJ( );
//	int a = test1.GetA( );
//	string b = test1.GetB( );
	CLog::CreateInstance( );
	HIREDIS_LOG_DEBUG("12313123123 %d",1);
	HIREDIS_LOG_ERROR("12313123123 %d",1);
}