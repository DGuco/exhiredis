//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RLONGLONG_H
#define EXHIREDIS_RLONGLONG_H

#include "robject.h"
namespace exhiredis
{
	class RLongLong: public Robject<long long>
	{
	public:
		RLongLong();
		RLongLong(long long value);
		void FromString(const string &str) override;
		const string ToString() override;
	};
}
#endif //EXHIREDIS_RLONGLONG_H
