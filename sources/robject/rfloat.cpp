//
// Created by dguco on 18-10-23.
//

#include "robject/rfloat.h"

namespace exhiredis
{
	RFloat::RFloat()
	{
		this->value = 0.0f;
	}

	RFloat::RFloat(float value)
	{
		this->value = value;
	}

	float RFloat::FromString(const string &str)
	{
		return static_cast<float>(atof(str.c_str( )));
	}
	const string RFloat::ToString()
	{
		return to_string(this->value);
	}
}