//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RULONG_H
#define EXHIREDIS_RULONG_H

#include <cstring>
#include "robject.hpp"

namespace exhiredis
{
	class RUlong: public Robject<unsigned long>
	{
	public:
		RUlong();
		RUlong(unsigned long value);
		void FromString(char *str, int len) override;
		const int ToString(char *str) override;
	};

	RUlong::RUlong()
	{
		this->value = 0;
	}
	RUlong::RUlong(unsigned long value)
	{
		this->value = value;
	}
	void RUlong::FromString(char *str, int len)
	{
		this->value = static_cast<unsigned long>(atol(str));
	}

	const int RUlong::ToString(char *str)
	{
		string strValue = to_string(this->value);
		strncpy(str, strValue.c_str( ), strValue.length( ));
		return strValue.length( );
	}
}

#endif //EXHIREDIS_RULONG_H
