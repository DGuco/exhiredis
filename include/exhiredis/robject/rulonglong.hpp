//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RULONGLONG_H
#define EXHIREDIS_RULONGLONG_H

#include <cstring>
#include "robject.hpp"

namespace exhiredis
{
	class RULongLong: public Robject<unsigned long long>
	{
	public:
		RULongLong();
		RULongLong(unsigned long long value);
		void FromString(char *str, int len) override;
		const int ToString(char *str) override;
	};

	RULongLong::RULongLong()
	{
		this->value = 0;
	}

	RULongLong::RULongLong(unsigned long long value)
	{
		this->value = value;
	}
	void RULongLong::FromString(char *str, int len)
	{
		this->value = static_cast<unsigned long long int>(atoll(str));
	}
	const int RULongLong::ToString(char *str)
	{
		string strValue = to_string(this->value);
		strncpy(str, strValue.c_str( ), strValue.length( ));
		return strValue.length( );
	}
}


#endif //EXHIREDIS_RULONGLONG_H
