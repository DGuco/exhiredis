//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RLONGLONG_H
#define EXHIREDIS_RLONGLONG_H

#include <cstring>
#include "robject.h"

namespace exhiredis
{
class RLongLong: public Robject<long long>
{
public:
    RLongLong()
    {
        this->value = 0ll;
    }

    RLongLong(long long value)
    {
        this->value = value;
    }

    void FromString(const string &str) override
    {
        this->value = stoll(str);
    }
    const string ToString() override
    {
        return to_string(this->value);
    }
};

}
#endif //EXHIREDIS_RLONGLONG_H
