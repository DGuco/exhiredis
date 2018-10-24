//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RMAP_H
#define EXHIREDIS_RMAP_H

#include <memory>
#include "robject/robject.h"
#include "rimap.h"
#include "redis_conn.h"

namespace exhiredis
{
	class RMap: public IMap<IRobject, IRobject>
	{
	public:
		RMap(std::shared_ptr<CRedisConn> conn);
		void Put(IRobject &key, IRobject &value);
		void Get(IRobject &key) override;
	private:
		std::shared_ptr<CRedisConn> m_pRedisConn;
	};
}
#endif //EXHIREDIS_RMAP_H
