//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RINT_HPP
#define EXHIREDIS_RINT_HPP

#include "robject.hpp"

namespace exhiredis {
	class RInt : public Robject<int>
	{
	public:
		RInt();
		RInt(int value);
		void FromString(const string &str) override;
		const string ToString() override;
	};

	RInt::RInt()
	{
		this->value = 0;
	}

	RInt::RInt(int value)
	{
		this->value = value;
	}

	void RInt::FromString(const string &str)
	{
		this->value = atoi(str.c_str( ));
	}

	const string RInt::ToString()
	{
		return to_string(this->value);
	}
}
#endif //EXHIREDIS_RINT_HPP
