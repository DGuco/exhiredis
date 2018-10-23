//
// Created by dguco on 18-10-23.
//

#include "robject/rlonglong.h"
namespace exhiredis
{
	RLongLong::RLongLong()
	{
		this->value = 0ll;
	}

	RLongLong::RLongLong(long long value)
	{
		this->value = value;
	}

	long long RLongLong::FromString(const string &str)
	{
		return atoll(str.c_str( ));
	}

	const string RLongLong::ToString()
	{
		return to_string(this->value);
	}
}
