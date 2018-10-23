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

	double RDouble::FromString(const string &str)
	{
		return nullptr;
	}
	
	const string RDouble::ToString()
	{
		return nullptr;
	}
}