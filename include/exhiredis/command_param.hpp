//
// Created by dguco on 18-12-27.
//

#ifndef EXHIREDIS_COMMAND_PARAM_HPP
#define EXHIREDIS_COMMAND_PARAM_HPP
#include <cstddef>
#include <cstring>
#include <memory>
#include <hiredis/sds.h>
#include "config/redis_config.hpp"
#include "utils/exception.hpp"

using namespace std;

namespace exhiredis
{
struct CCmdParam
{
    CCmdParam()
    {
        this->m_data = new char[REDIS_MAX_KEY_VALUE_SIZE];
    }

    CCmdParam(const char *data, int len)
    {
        this->m_data = new char[REDIS_MAX_KEY_VALUE_SIZE];
        memcpy(m_data, data, len);
    }

    CCmdParam(const string &str)
    {
        this->m_data = new char[REDIS_MAX_KEY_VALUE_SIZE];
        memcpy(m_data, str.c_str(), str.length());
    }

    CCmdParam(size_t size)
    {
        this->m_len = size;
    }

    virtual ~CCmdParam()
    {
        if (m_data != nullptr) {
            delete[](m_data);
        }
    }

    char *GetData()
    {
        if (m_data != nullptr) {
            return m_data;
        }
        else {
            throw CRException("The cmd param is illegal");
        }
    }

    size_t GetLen()
    {
        return m_len;
    }

    CCmdParam(const CCmdParam &parm) = delete;
    CCmdParam(const CCmdParam &&parm) = delete;
    CCmdParam &operator=(const CCmdParam &parm) = delete;
    char *m_data;
    size_t m_len;
};
}
#endif //EXHIREDIS_COMMAND_PARAM_HPP
