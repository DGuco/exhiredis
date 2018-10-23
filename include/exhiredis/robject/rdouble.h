//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RFLOAT_H
#define EXHIREDIS_RFLOAT_H

#include "robject.h"
namespace exhiredis
{
	class RDouble: public IRobject<double>
	{
	public:
		RDouble();
		RDouble(double value);
		double FromString(const string &str) override;
		const string ToString() override;

	};
}
#endif //EXHIREDIS_RFLOAT_H
