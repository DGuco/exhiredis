//
// Created by dguco on 18-10-23.
//

#include "robject/rdouble.h"

namespace exhiredis
{
	RDouble::RDouble()
	{
		this->value = 0.0f;
	}

	RDouble::RDouble(double value)
	{
		this->value = value;
	}

	void RDouble::FromString(const string &str)
	{
		this->value = atof(str.c_str( ));
	}

	const string RDouble::ToString()
	{
		return to_string(this->value);
	}
}