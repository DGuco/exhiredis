//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RUINT_HPP
#define EXHIREDIS_RUINT_HPP

#include <cstring>
#include "robject.hpp"

namespace exhiredis
{
	class RUint: public Robject<unsigned int>
	{
	public:
		RUint();
		RUint(unsigned int value);
		void FromString(char *str, int len) override;
		const int ToString(char *str) override;
	};

	RUint::RUint()
	{
		this->value = 0;
	}
	RUint::RUint(unsigned int value)
	{
		this->value = value;
	}
	void RUint::FromString(char *str, int len)
	{
		this->value = static_cast<unsigned int>(atoi(str));
	}

	const int RUint::ToString(char *str)
	{
		string strValue = to_string(this->value);
		strncpy(str, strValue.c_str( ), strValue.length( ));
		return strValue.length( );
	}
}

#endif //EXHIREDIS_RUINT_HPP
