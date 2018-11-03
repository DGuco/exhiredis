//
// Created by dguco on 18-10-18.
// libuuid
//

#ifndef EXHIREDIS_UUID_HPP
#define EXHIREDIS_UUID_HPP

#include <uuid/uuid.h>

namespace exhiredis
{
	class CUuid final
	{
	public:
		const char *GenerateUuid();
	private:
		uuid_t uuid;
		char buff[1024];
	};

	const char *CUuid::GenerateUuid()
	{
		uuid_clear(uuid);
		uuid_generate(uuid);
		uuid_unparse(uuid, buff);
		return buff;
	}
}
#endif //EXHIREDIS_UUID_HPP
