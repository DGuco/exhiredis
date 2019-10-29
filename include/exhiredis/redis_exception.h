//
// Created by dguco on 19-10-29.
//

#ifndef EXHIREDIS_REDIS_EXCEPTION_H
#define EXHIREDIS_REDIS_EXCEPTION_H

#include <exception>
#include <string>
using namespace std;

namespace exhiredis
{
    class CRedisException: public std::exception
    {
    public:
        CRedisException(const std::string& what): _what(what){}
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
