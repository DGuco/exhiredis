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
	void RLong::FromString(const string &str)
	{
		this->value = atol(str.c_str( ));
	}
	const string RLong::ToString()
	{
		return to_string(this->value);
	}
}