//
// Created by dguco on 19-10-29.
//

#ifndef EXHIREDIS_REDIS_EXCEPTION_H
#define EXHIREDIS_REDIS_EXCEPTION_H

#include <exception>
#include <string>
#include <utility>

using namespace std;

namespace exhiredis
{
    class CRedisException: public std::exception
    {
    public:
        CRedisException(std::string what) noexcept : _what(std::move(what))
        {

        }

        CRedisException(const CRedisException& tmp) noexcept
        {
            _what = tmp._what;
        }

        virtual ~CRedisException() throw() {}

        inline virtual const char* what() const noexcept override
        {
            return _what.c_str();
        }
    private:
        std::string _what;
    };
}

#endif //EXHIREDIS_REDIS_EXCEPTION_H
