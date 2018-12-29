//
// Created by dguco on 18-10-23.
//

#ifndef EXHIREDIS_ROBJECT_H
#define EXHIREDIS_ROBJECT_H

#include <string>
#include <memory>

using namespace std;

namespace exhiredis
{
class IRobject
{
public:
    IRobject()
    {

    };
    virtual void FromString(const string &str)  = 0;
    virtual const string ToString()  = 0;
};

template<class T>
class Robject: public IRobject
{
public:
    /**
     *
     * @param str
     * @param len  char* len
     */
    virtual void FromString(const string &str)  = 0;
    /**
     *
     * @param str
     * @return  char* len
     */
    virtual const string ToString()  = 0;
    T &Value()
    {
        return value;
    };
protected:
    T value;
};
}
#endif //EXHIREDIS_ROBJECT_H
