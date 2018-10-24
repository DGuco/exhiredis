//
// Created by dguco on 18-10-23.
//

#include "robject/rstring.h"

namespace exhiredis
{
	RString::RString()
	{
		this->value = "";
	}

	RString::RString(string str)
	{
		this->value = value;
	}
	void RString::FromString(const string &str)
	{
		this->value = str;
	}
	const string RString::ToString()
	{
		return this->value;
	}
}

