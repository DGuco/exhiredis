//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RINT_HPP
#define EXHIREDIS_RINT_HPP

#include <cstring>
#include "robject.hpp"

namespace exhiredis
{
class RLong: public Robject<long>
{
public:
    RLong();
    RLong(long value);
    void FromString(char *str, int len) override;
    const int ToString(char *str) override;
};

RLong::RLong()
{
    this->value = 0;
}

RLong::RLong(long value)
{
    this->value = value;
}

void RLong::FromString(char *str, int len)
{
    this->value = atol(str);
}

const int RLong::ToString(char *str)
{
    string strValue = to_string(this->value);
    strncpy(str, strValue.c_str(), strValue.length());
    return strValue.length();
}
}
#endif //EXHIREDIS_RINT_HPP
