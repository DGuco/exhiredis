//
// Created by dguco on 18-10-23.
//

#include "robject/robject.h"
namespace exhiredis
{
	template<class T>
	T &IRobject<T>::Value()
	{
		return value;
	}
}