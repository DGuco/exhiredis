//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RUINT_HPP
#define EXHIREDIS_RUINT_HPP

#include "robject.hpp"
namespace exhiredis
{
	class RUint: public Robject<unsigned int>
	{
	public:
		RUint();
		RUint(unsigned int value);
		void FromString(const string &str) override;
		const string ToString() override;
	};

	RUint::RUint()
	{
		this->value = 0;
	}
	RUint::RUint(unsigned int value)
	{
		this->value = value;
	}
	void RUint::FromString(const string &str)
	{
		this->value = static_cast<unsigned int>(atoi(str.c_str( )));
	}

	const string RUint::ToString()
	{
		return to_string(this->value);
	}
}

#endif //EXHIREDIS_RUINT_HPP
