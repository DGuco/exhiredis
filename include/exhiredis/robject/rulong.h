//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RULONG_H
#define EXHIREDIS_RULONG_H

#include "robject.h"
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
}

#endif //EXHIREDIS_RULONG_H
