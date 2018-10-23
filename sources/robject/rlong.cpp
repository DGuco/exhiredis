//
// Created by dguco on 18-10-23.
//

#include "robject/rlong.h"

namespace exhiredis
{
	RLong::RLong()
	{
		this->value = 0;
	}
	RLong::RLong(long value)
	{
		this->value = value;
	}
	long RLong::FromString(const string &str)
	{
		return atol(str.c_str( ));
	}
	const string RLong::ToString()
	{
		return to_string(this->value);
	}
}