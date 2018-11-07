//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RINT_HPP
#define EXHIREDIS_RINT_HPP

#include <cstring>
#include "robject.hpp"

namespace exhiredis
{
class RInt: public Robject<int>
{
public:
    RInt();
    RInt(int value);
    void FromString(char *str, int len) override;
    const int ToString(char *str) override;
};

RInt::RInt()
{
    this->value = 0;
}

RInt::RInt(int value)
{
    this->value = value;
}

void RInt::FromString(char *str, int len)
{
    this->value = atoi(str);
}

const int RInt::ToString(char *str)
{
    string strValue = to_string(this->value);
    strncpy(str, strValue.c_str(), strValue.length());
    return strValue.length();
}
}
#endif //EXHIREDIS_RINT_HPP
