//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RFLOAT_H
#define EXHIREDIS_RFLOAT_H

#include "robject.hpp"
namespace exhiredis
{
	class RDouble: public Robject<double>
	{
	public:
		RDouble();
		RDouble(double value);
		void FromString(const string &str) override;
		const string ToString() override;

	};

	RDouble::RDouble()
	{
		this->value = 0.0f;
	}

	RDouble::RDouble(double value)
	{
		this->value = value;
	}

	void RDouble::FromString(const string &str)
	{
		this->value = atof(str.c_str( ));
	}

	const string RDouble::ToString()
	{
		return to_string(this->value);
	}
}
#endif //EXHIREDIS_RFLOAT_H
