//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RSTRING_H
#define EXHIREDIS_RSTRING_H

#include <cstring>
#include <string>
#include "robject.h"

namespace exhiredis
{
class RString: public Robject<string>
{
public:
    RString()
    {
        this->value = "";
    }

    RString(string str)
    {
        this->value = value;
    }

    void FromString(const string &str) override
    {
        this->value = str;
    }
    const string ToString() override
    {
        return this->value;
    }
};

}
#endif //EXHIREDIS_RSTRING_H
