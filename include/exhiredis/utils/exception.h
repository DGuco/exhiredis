//
// Created by dguco on 18-12-25.
//

#ifndef EXHIREDIS_EXCEPTION_HPP
#define EXHIREDIS_EXCEPTION_HPP
#include <exception>
#include <string>
#include <execinfo.h>
#include <cstring>

using namespace std;
namespace exhiredis
{
class CRException: public exception
{
public:
    CRException(const string &buffer)
        : m_sBuffer(buffer),
          m_iCode(0)
    {
        GenBacktrace();
    }

    CRException(const string &buffer, int err)
    {
        m_sBuffer = buffer + " :" + strerror(err);
        m_iCode = err;
        GenBacktrace();
    }

    ~CRException()
    {
    }

    int GetErrCode() const
    {
        return m_iCode;
    }

    const char *what()
    {
        return m_sBuffer.c_str();
    }

    void GenBacktrace()
    {
        void *array[64];
        int nSize = backtrace(array, 64);
        char **symbols = backtrace_symbols(array, nSize);

        for (int i = 0; i < nSize; i++) {
            m_sBuffer += symbols[i];
            m_sBuffer += "\n";
        }
        free(symbols);
    }
private:
    string m_sBuffer;
    int m_iCode;
};
}
#endif //EXHIREDIS_EXCEPTION_HPP
