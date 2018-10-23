//
// Created by dguco on 18-10-23.
//

#include "robject/rulong.h"

namespace exhiredis
{
	RUlong::RUlong()
	{
		this->value = 0;
	}
	RUlong::RUlong(unsigned long value)
	{
		this->value = value;
	}
	unsigned long RUlong::FromString(const string &str)
	{
		return static_cast<unsigned long>(atol(str.c_str( )));
	}
	const string RUlong::ToString()
	{
		return to_string(this->value);
	}
}