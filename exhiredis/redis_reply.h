//
// Created by dguco on 19-10-25.
//

#ifndef EXHIREDIS_REDIS_REPLY_H
#define EXHIREDIS_REDIS_REPLY_H

#include <string>
#include <vector>
#include "../hiredis/hiredis.h"
#include "redis_exception.h"
#include "rstl/param.h"

using namespace std;

namespace exhiredis
{
    class CRedisReply
    {
    public:
        enum class eReplyType
        {
            STRING = 1,
            ARRAY = 2,
            INTEGER = 3,
            NIL = 4,
            STATUS = 5,
            ERROR = 6
        };


        inline eReplyType ReplyType() const { return m_type; }

        inline const std::string &StrValue() const { return m_str; }

        inline long long IntegerValue() const { return m_integer; }

        inline const std::vector<CRedisReply> &ArrayElements() const { return m_elements; }

        inline operator const std::string &() const { return m_str; }

        inline operator long long() const { return m_integer; }

        inline bool operator==(const std::string &rvalue) const
        {
            if (m_type == eReplyType::STRING || m_type == eReplyType::ERROR || m_type == eReplyType::STATUS) {
                return m_str == rvalue;
            } else {
                return false;
            }
        }

        inline bool operator==(const long long rvalue) const
        {
            if (m_type == eReplyType::INTEGER)
            {
                return m_integer == rvalue;
            } else {
                return false;
            }
        }


        CRedisReply(redisReply *reply);

        CRedisReply();

        template<typename param_type>
        void ParseToParam(CParam<param_type> &param) const
        {
            if (m_type == eReplyType::ERROR)
            {
                throw  CRedisException("Redis reply type is error");
            }

            if (m_type == eReplyType::STRING)
            {
                param.FromString(StrValue());
            }
        }

        void ParseToParam(CParam<bool> &param) const
        {
            if (m_type == eReplyType::ERROR)
            {
                throw  CRedisException("Redis reply type is error");
            }

            //if param_type is bool
            if (m_type == eReplyType::INTEGER)
            {
                param.value = IntegerValue() == 1;
                return;
            }

            if (m_type == eReplyType::STRING)
            {
                param.FromString(StrValue());
            }

        }

    private:
        eReplyType m_type;
        std::string m_str;
        long long m_integer;
        std::vector <CRedisReply> m_elements;
    };
}
#endif //EXHIREDIS_REDIS_REPLY_H
