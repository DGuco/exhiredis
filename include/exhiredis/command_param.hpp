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
    CCmdParam(const string &str)
    {
        this->m_data = new char[str.length() + 1]{0};
        //binary safe 二进制安全
        memcpy(m_data, str.c_str(), str.length());
        //memcpy(m_data, str.c_str(), strlen(str.c_str())); binary not safe 非二进制安全
    }

    CCmdParam(const char *data, int len)
    {
        this->m_data = new char[len + 1]{0};
        memcpy(m_data, data, len);
    }

    CCmdParam(size_t size)
    {
        InitData<size_t>(size);
    }

    CCmdParam(int param)
    {
        InitData<size_t>(param);
    }

    char *GetChar()
    {
        if (m_data != nullptr) {
            return m_data;
        }
    }

    size_t GetSize()
    {
        return GetParam<size_t>();
    }

    virtual ~CCmdParam()
    {
        if (m_data != nullptr) {
            delete[](m_data);
        }
    }

private:
    template<class T>
    void InitData(T t)
    {
        this->m_data = new char[sizeof(t)]{0};
        memcpy(m_data, (void *) (&t), sizeof(t));
    }
    template<class T>
    T GetParam()
    {
        return *((T *) (m_data));
    }
    CCmdParam(const CCmdParam &parm) = delete;
    CCmdParam(const CCmdParam &&parm) = delete;
    CCmdParam &operator=(const CCmdParam &parm) = delete;
    char *m_data;
};
}
#endif //EXHIREDIS_COMMAND_PARAM_HPP
