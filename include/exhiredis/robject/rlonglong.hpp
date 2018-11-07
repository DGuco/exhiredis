//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RLONGLONG_H
#define EXHIREDIS_RLONGLONG_H

#include <cstring>
#include "robject.hpp"

namespace exhiredis
{
class RLongLong: public Robject<long long>
{
public:
    RLongLong();
    RLongLong(long long value);
    void FromString(char *str, int len) override;
    const int ToString(char *str) override;
};

RLongLong::RLongLong()
{
    this->value = 0ll;
}

RLongLong::RLongLong(long long value)
{
    this->value = value;
}

void RLongLong::FromString(char *str, int len)
{
    this->value = atoll(str);
}

const int RLongLong::ToString(char *str)
{
    string strValue = to_string(this->value);
    strncpy(str, strValue.c_str(), strValue.length());
    return strValue.length();
}

}
#endif //EXHIREDIS_RLONGLONG_H
