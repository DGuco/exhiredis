//
// Created by dguco on 19-1-4.
//

#include "log.h"

namespace exhiredis
{
mutex *CLog::m_logLock = new mutex;
}