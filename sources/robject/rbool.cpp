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

	void RBool::FromString(const string &str)
	{
		this->value = str == "true";
	}
	
	const string RBool::ToString()
	{
		if (this->value) {
			return "true";
		}
		else {
			return "false";
		}
	}
}