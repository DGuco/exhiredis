//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_ROBJECT_H
#define EXHIREDIS_ROBJECT_H

#include <string>
#include <memory>

using namespace std;

namespace exhiredis
{
	class IRobject
	{
	public:
		virtual void FromString(char *str, int len)  = 0;
		virtual const int ToString(char *str)  = 0;
	};

	template<class T>
	class Robject: public IRobject
	{
	public:
		/**
		 *
		 * @param str
		 * @param len  char* len
		 */
		virtual void FromString(char *str, int len)  = 0;
		/**
		 *
		 * @param str
		 * @return  char* len
		 */
		virtual const int ToString(char *str)  = 0;
		T &Value()
		{
			return value;
		};
	protected:
		T value;
	};
}
#endif //EXHIREDIS_ROBJECT_H
