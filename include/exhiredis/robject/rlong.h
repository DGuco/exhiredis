//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RINT_HPP
#define EXHIREDIS_RINT_HPP

#include <cstring>
#include "robject.h"

namespace exhiredis
{
class RLong: public Robject<long>
{
public:
    RLong()
    {
        this->value = 0;
    }

    RLong(long value)
    {
        this->value = value;
    }

    void FromString(const string &str) override
    {
        this->value = stol(str);
    }
    const string ToString() override
    {
        return to_string(this->value);
    }
};

}
#endif //EXHIREDIS_RINT_HPP
