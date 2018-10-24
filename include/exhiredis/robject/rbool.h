//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RBOOL_H
#define EXHIREDIS_RBOOL_H
#include "robject.h"
namespace exhiredis
{
	class RBool: public Robject<bool>
	{
	public:
		RBool();
		RBool(bool value);
		void  FromString(const string &str) override;
		const string ToString() override;
	};
}
#endif //EXHIREDIS_RBOOL_H
