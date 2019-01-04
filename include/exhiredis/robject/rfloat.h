//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RFLOAT_H
#define EXHIREDIS_RFLOAT_H

#include <cstring>
#include <string>
#include "robject.h"

namespace exhiredis
{
class RFloat: public Robject<float>
{
public:
    RFloat()
    {
        this->value = 0.0f;
    }

    RFloat(float value)
    {
        this->value = value;
    }

    void FromString(const string &str) override
    {
        this->value = stof(str);
    }
    const string ToString() override
    {
        return to_string(this->value);
    }
};


}
#endif //EXHIREDIS_RFLOAT_H
