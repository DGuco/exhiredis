//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RINT_HPP
#define EXHIREDIS_RINT_HPP

#include <cstring>
#include "robject.h"

namespace exhiredis
{
class RInt: public Robject<int>
{
public:
    RInt()
    {
        this->value = 0;
    }

    RInt(int value)
    {
        this->value = value;
    }

    void FromString(const string &str) override
    {
        this->value = stoi(str);
    }

    const string ToString() override
    {
        return to_string(this->value);
    }
};


}
#endif //EXHIREDIS_RINT_HPP
