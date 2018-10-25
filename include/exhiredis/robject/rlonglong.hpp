//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RLONGLONG_H
#define EXHIREDIS_RLONGLONG_H

#include "robject.hpp"
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

	RLongLong::RLongLong()
	{
		this->value = 0ll;
	}

	RLongLong::RLongLong(long long value)
	{
		this->value = value;
	}

	void RLongLong::FromString(const string &str)
	{
		this->value = atoll(str.c_str( ));
	}

	const string RLongLong::ToString()
	{
		return to_string(this->value);
	}
}
#endif //EXHIREDIS_RLONGLONG_H
