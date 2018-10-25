//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RINT_HPP
#define EXHIREDIS_RINT_HPP

#include "robject.hpp"

namespace exhiredis
{
	class RLong: public Robject<long>
	{
	public:
		RLong();
		RLong(long value);
		void FromString(const string &str) override;
		const string ToString() override;
	};

	RLong::RLong()
	{
		this->value = 0;
	}
	RLong::RLong(long value)
	{
		this->value = value;
	}
	void RLong::FromString(const string &str)
	{
		this->value = atol(str.c_str( ));
	}
	const string RLong::ToString()
	{
		return to_string(this->value);
	}
}
#endif //EXHIREDIS_RINT_HPP
