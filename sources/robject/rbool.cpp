//
// Created by dguco on 18-10-23.
//
#include "robject/rbool.h"
namespace exhiredis
{
	RBool::RBool()
	{
		this->value = false;
	}

	RBool::RBool(bool value)
	{
		this->value = value;
	}

	bool RBool::FromString(const string &str)
	{
		int value = atoi(str.c_str( ));
		return value != 0;
	}
	const string RBool::ToString()
	{
		if (this->value) {
			return "1";
		}
		else {
			return 0;
		}
	}
}