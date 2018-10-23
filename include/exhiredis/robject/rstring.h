//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RSTRING_H
#define EXHIREDIS_RSTRING_H

#include "robject.h"
namespace exhiredis
{
	class RString: public IRobject<string>
	{
	public:
		RString();
		RString(string str);
		string FromString(const string &str) override;
		const string ToString() override;
	};
}
#endif //EXHIREDIS_RSTRING_H
