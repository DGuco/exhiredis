//
// Created by dguco on 18-10-20.
//

#include "utils/uuid.h"

namespace exhiredis
{
	const char *CUuid::GenerateUuid()
	{
		uuid_clear(uuid);
		uuid_generate(uuid);
		uuid_unparse(uuid, buff);
		return buff;
	}
}