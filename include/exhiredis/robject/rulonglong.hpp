//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RULONGLONG_H
#define EXHIREDIS_RULONGLONG_H

#include "robject.hpp"
namespace exhiredis
{
	class RULongLong: public Robject<unsigned long long>
	{
	public:
		RULongLong();
		RULongLong(unsigned long long value);
		void FromString(const string &str) override;
		const string ToString() override;
	};

	RULongLong::RULongLong()
	{
		this->value = 0;
	}

	RULongLong::RULongLong(unsigned long long value)
	{
		this->value = value;
	}
	void RULongLong::FromString(const string &str)
	{
		this->value = static_cast<unsigned long long int>(atoll(str.c_str( )));
	}

	const string RULongLong::ToString()
	{
		return to_string(this->value);
	}
}


#endif //EXHIREDIS_RULONGLONG_H
