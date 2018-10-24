//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RMAP_HPP
#define EXHIREDIS_RMAP_HPP
#include "robject/robject.h"

namespace exhiredis
{
	template<class key_type, class value_type>
	class IMap
	{
	public:
		virtual void Put(key_type &key, value_type &value) = 0;
		virtual void Get(key_type &key) = 0;
	};
}
#endif //EXHIREDIS_RMAP_HPP
