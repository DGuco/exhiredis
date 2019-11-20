//
// Created by dguco on 19-10-25.
//

#include <exhiredis/rstl/param.h>
#include "exhiredis/redis_reply.h"

namespace exhiredis
{
    CRedisReply::CRedisReply(redisReply *reply)
        : m_type(eReplyType::ERROR), m_integer(0)
    {
        m_type = static_cast<eReplyType>(reply->type);
        switch (m_type)
        {
            case eReplyType::ERROR:
            case eReplyType::STRING:
            case eReplyType::STATUS:
                m_str = std::string(reply->str, reply->len);
                break;
            case eReplyType::INTEGER:
                m_integer = reply->integer;
                break;
            case eReplyType::ARRAY:
                for (size_t i=0; i < reply->elements; ++i)
                {
                    m_elements.push_back(CRedisReply(reply->element[i]));
                }
                break;
            default:
                break;
        }
    }

    CRedisReply::CRedisReply()
            : m_type(eReplyType::ERROR), m_integer(0)
    {}
}