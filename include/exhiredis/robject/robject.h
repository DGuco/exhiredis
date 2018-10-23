//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_ROBJECT_H
#define EXHIREDIS_ROBJECT_H

#include <string>
using namespace std;

namespace exhiredis
{
	template<class T>
	class IRobject
	{
	public:
		virtual T FromString(const string &str) = 0;
		virtual const string ToString() = 0;
		T &Value();
	protected:
		T value;
	};
}
#endif //EXHIREDIS_ROBJECT_H
