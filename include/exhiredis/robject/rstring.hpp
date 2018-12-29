//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RSTRING_H
#define EXHIREDIS_RSTRING_H

#include <cstring>
#include <string>
#include "robject.hpp"

namespace exhiredis
{
class RString: public Robject<string>
{
public:
    RString();
    RString(string str);
    void FromString(const string &str) override;
    const string ToString() override;
};

RString::RString()
{
    this->value = "";
}

RString::RString(string str)
{
    this->value = value;
}

void RString::FromString(const string &str)
{
    this->value = str;
}
const string RString::ToString()
{
    return this->value;
}
}
#endif //EXHIREDIS_RSTRING_H
