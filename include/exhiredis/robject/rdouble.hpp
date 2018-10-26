//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RFLOAT_H
#define EXHIREDIS_RFLOAT_H

#include <cstring>
#include "robject.hpp"
namespace exhiredis
{
	class RDouble: public Robject<double>
	{
	public:
		RDouble();
		RDouble(double value);
		void FromString(char *str, int len) override;
		const int ToString(char *str) override;
	};

	RDouble::RDouble()
	{
		this->value = 0.0f;
	}

	RDouble::RDouble(double value)
	{
		this->value = value;
	}

	void RDouble::FromString(char *str, int len)
	{
		this->value = atof(str);
	}

	const int RDouble::ToString(char *str)
	{
		string strValue = to_string(this->value);
		strncpy(str, strValue.c_str( ), strValue.length( ));
		return strValue.length( );
	}
}
#endif //EXHIREDIS_RFLOAT_H
