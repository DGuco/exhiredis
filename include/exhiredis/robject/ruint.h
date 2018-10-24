//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RUINT_HPP
#define EXHIREDIS_RUINT_HPP

#include "robject.h"
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
}

#endif //EXHIREDIS_RUINT_HPP
