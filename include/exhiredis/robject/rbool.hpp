//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RBOOL_H
#define EXHIREDIS_RBOOL_H

#include <cstring>
#include "robject.hpp"

namespace exhiredis
{
class RBool: public Robject<bool>
{
public:
    RBool();
    RBool(bool value);
    void FromString(const string &str) override;
    const string ToString() override;
};

RBool::RBool()
{
    this->value = false;
}

RBool::RBool(bool value)
{
    this->value = value;
}

void RBool::FromString(const string &str)
{
    this->value = strcmp(str.c_str(), "true") == 0;
}

const string RBool::ToString()
{
    string strValue;
    if (this->value) {
        return "true";
    }
    else {
        return "false";
    }
}
}
#endif //EXHIREDIS_RBOOL_H
