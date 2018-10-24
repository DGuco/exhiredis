//
// Created by dguco on 18-10-23.
//
#include "robject/rulonglong.h"

namespace exhiredis
{
	RULongLong::RULongLong()
	{
		this->value = 0;
	}

	RULongLong::RULongLong(unsigned long long value)
	{
		this->value = value;
	}
	void RULongLong::FromString(const string &str)
	{
		this->value = static_cast<unsigned long long int>(atoll(str.c_str( )));
	}
	
	const string RULongLong::ToString()
	{
		return to_string(this->value);
	}

}