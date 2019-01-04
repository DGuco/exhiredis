//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RULONGLONG_H
#define EXHIREDIS_RULONGLONG_H

#include <cstring>
#include "robject.h"

namespace exhiredis
{
class RULongLong: public Robject<unsigned long long>
{
public:
    RULongLong()
    {
        this->value = 0;
    }

    RULongLong(unsigned long long value)
    {
        this->value = value;
    }
    void FromString(const string &str) override
    {
        this->value = stoull(str);
    }

    const string ToString() override
    {
        return to_string(this->value);
    }
};


}

#endif //EXHIREDIS_RULONGLONG_H
