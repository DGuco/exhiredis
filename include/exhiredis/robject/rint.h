//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RINT_HPP
#define EXHIREDIS_RINT_HPP

#include "robject.h"

namespace exhiredis {
	class RInt : public Robject<int>
	{
	public:
		RInt();
		RInt(int value);
		void FromString(const string &str) override;
		const string ToString() override;
	};
}
#endif //EXHIREDIS_RINT_HPP
