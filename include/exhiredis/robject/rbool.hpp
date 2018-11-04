//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RBOOL_H
#define EXHIREDIS_RBOOL_H

#include <cstring>
#include "robject.hpp"

namespace exhiredis
{
	class RBool: public Robject<bool>
	{
	public:
		RBool();
		RBool(bool value);
		void FromString(char *str, int len) override;
		const int ToString(char *str) override;
	};

	RBool::RBool()
	{
		this->value = false;
	}

	RBool::RBool(bool value)
	{
		this->value = value;
	}

	void RBool::FromString(char *str, int len)
	{
		this->value = strcmp(str, "true") == 0;
	}

	const int RBool::ToString(char *str)
	{
		string strValue;
		if (this->value) {
			strValue = "true";
		}
		else {
			strValue = "false";
		}
		strncpy(str, strValue.c_str( ), strValue.length( ));
		return strValue.length( );
	}
}
#endif //EXHIREDIS_RBOOL_H
