//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_RBOOL_H
#define EXHIREDIS_RBOOL_H

#include <cstring>
#include "robject.h"

namespace exhiredis
{
class RBool: public Robject<bool>
{
public:

    RBool()
    {
        this->value = false;
    }

    RBool(bool value)
    {
        this->value = value;
    }

    void FromString(const string &str) override
    {
        this->value = strcmp(str.c_str(), "true") == 0;
    }

    const string ToString() override
    {
        string strValue;
        if (this->value) {
            return "true";
        }
        else {
            return "false";
        }
    }
};
}
#endif //EXHIREDIS_RBOOL_H
