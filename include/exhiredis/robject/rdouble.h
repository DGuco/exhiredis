//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RFLOAT_H
#define EXHIREDIS_RFLOAT_H

#include <cstring>
#include "robject.h"

namespace exhiredis
{
class RDouble: public Robject<double>
{
public:
    RDouble()
    {
        this->value = 0.0f;
    }

    RDouble(double value)
    {
        this->value = value;
    }

    void FromString(const string &str) override
    {
        this->value = stod(str);
    }

    const string ToString() override
    {
        return to_string(this->value);
    }
};
}
#endif //EXHIREDIS_RFLOAT_H
