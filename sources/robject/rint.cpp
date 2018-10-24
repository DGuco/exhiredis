//
// Created by dguco on 18-10-23.
//

#include "robject/rint.h"

namespace exhiredis
{
	RInt::RInt()
	{
		this->value = 0;
	}

	RInt::RInt(int value)
	{
		this->value = value;
	}

	void RInt::FromString(const string &str)
	{
		this->value = atoi(str.c_str( ));
	}

	const string RInt::ToString()
	{
		return to_string(this->value);
	}
}