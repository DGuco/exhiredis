//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RULONG_H
#define EXHIREDIS_RULONG_H

#include "robject.hpp"
namespace exhiredis
{
	class RUlong: public Robject<unsigned long>
	{
	public:
		RUlong();
		RUlong(unsigned long value);
		void FromString(const string &str) override;
		const string ToString() override;
	};

	RUlong::RUlong()
	{
		this->value = 0;
	}
	RUlong::RUlong(unsigned long value)
	{
		this->value = value;
	}
	void RUlong::FromString(const string &str)
	{
		this->value = static_cast<unsigned long>(atol(str.c_str( )));
	}
	const string RUlong::ToString()
	{
		return to_string(this->value);
	}
}

#endif //EXHIREDIS_RULONG_H
