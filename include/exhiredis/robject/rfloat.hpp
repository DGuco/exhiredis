//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RFLOAT_H
#define EXHIREDIS_RFLOAT_H

#include <cstring>
#include <string>
#include "robject.hpp"

namespace exhiredis
{
class RFloat: public Robject<float>
{
public:
    RFloat();

    RFloat(float value);

    void FromString(char *str, int len) override;

    const int ToString(char *str) override;
};

RFloat::RFloat()
{
    this->value = 0.0f;
}

RFloat::RFloat(float value)
{
    this->value = value;
}

void RFloat::FromString(char *str, int len)
{
    this->value = static_cast<float>(atof(str));
}

const int RFloat::ToString(char *str)
{
    string strValue = to_string(this->value);
    memcpy(str, strValue.c_str(), strValue.length());
    return strValue.length();
}
}
#endif //EXHIREDIS_RFLOAT_H
