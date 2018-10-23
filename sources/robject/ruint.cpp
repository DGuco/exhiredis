//
// Created by dguco on 18-10-23.
//

#include "robject/ruint.h"
namespace exhiredis
{

	RUint::RUint()
	{
		this->value = 0;
	}
	RUint::RUint(unsigned int value)
	{
		this->value = value;
	}
	unsigned int RUint::FromString(const string &str)
	{
		this->value = static_cast<unsigned int>(atoi(str.c_str( )));
		return this->value;
	}
	
	const string RUint::ToString()
	{
		return to_string(this->value);
	}
}