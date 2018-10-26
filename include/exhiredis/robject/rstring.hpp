//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RSTRING_H
#define EXHIREDIS_RSTRING_H

#include <cstring>
#include "robject.hpp"
namespace exhiredis
{
	class RString: public Robject<string>
	{
	public:
		RString();
		RString(string str);
		void FromString(char *str, int len) override;
		const int ToString(char *str) override;
	};

	RString::RString()
	{
		this->value = "";
	}

	RString::RString(string str)
	{
		this->value = value;
	}

	void RString::FromString(char *str, int len)
	{
		this->value = str;
	}

	const int RString::ToString(char *str)
	{
		strncpy(str, this->value.c_str( ), this->value.length( ));
		return this->value.length( );
	}
}
#endif //EXHIREDIS_RSTRING_H
