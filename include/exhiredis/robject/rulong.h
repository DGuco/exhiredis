//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RULONG_H
#define EXHIREDIS_RULONG_H

#include <cstring>
#include "robject.h"

namespace exhiredis
{
class RUlong: public Robject<unsigned long>
{
public:
    RUlong()
    {
        this->value = 0;
    }

    RUlong(unsigned long value)
    {
        this->value = value;
    }

    void FromString(const string &str) override
    {
        this->value = stoul(str);
    }
    const string ToString() override
    {
        return to_string(this->value);
    }

};

}

#endif //EXHIREDIS_RULONG_H
