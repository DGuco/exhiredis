//
// Created by dguco on 18-10-18.
// 生成全球唯一id 依赖libuuid
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
}
#endif //EXHIREDIS_UUID_HPP
