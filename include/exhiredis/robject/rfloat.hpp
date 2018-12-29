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
    void FromString(const string &str) override;
    const string ToString() override;
};

RFloat::RFloat()
{
    this->value = 0.0f;
}

RFloat::RFloat(float value)
{
    this->value = value;
}

void RFloat::FromString(const string &str)
{
    this->value = stof(str);
}
const string RFloat::ToString()
{
    return to_string(this->value);
}

}
#endif //EXHIREDIS_RFLOAT_H
