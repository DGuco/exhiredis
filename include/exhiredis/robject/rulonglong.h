//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RULONGLONG_H
#define EXHIREDIS_RULONGLONG_H

#include "robject.h"
namespace exhiredis
{
	class RULongLong: public IRobject<unsigned long long>
	{
	public:
		RULongLong();
		RULongLong(unsigned long long value);
		unsigned long long FromString(const string &str) override;
		const string ToString() override;
	};
}


#endif //EXHIREDIS_RULONGLONG_H
