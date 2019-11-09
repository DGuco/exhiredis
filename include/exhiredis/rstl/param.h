//
// redis param
// Created by dguco on 19-11-9.
//

#ifndef EXHIREDIS_PARAM_H
#define EXHIREDIS_PARAM_H

#include <string>

using namespace std;

namespace exhiredis
{
    template <class T>
    struct CParam
    {
        CParam(T t) {}
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
        T value;
    };

    /**
     * template param<string>
     */
    template <>
    struct CParam<string>
    {
        CParam()
        {
            this->value = 0;
        }

        CParam(int value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = str;
        }

        const string ToString()
        {
            return this->value;
        }
        string value;
    };

    /**
     * template param<int>
     */
    template <>
    struct CParam<int>
    {
        CParam()
        {
            this->value = 0;
        }

        CParam(int value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = stoi(str);
        }

        const string ToString()
        {
            return to_string(this->value);
        }
        int value;
    };

    /**
     * template param<unsigned int>
     */
    template <>
    struct CParam<unsigned int>
    {
        CParam()
        {
            this->value = 0;
        }

        CParam(unsigned int value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = static_cast<unsigned int>(stol(str));
        }

        const string ToString()
        {
            return to_string(this->value);
        }
        unsigned int value;
    };

    /**
     * template param<long>
     */
    template <>
    struct CParam<long>
    {
        CParam()
        {
            this->value = 0;
        }

        CParam(long value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = stol(str);
        }

        const string ToString()
        {
            return to_string(this->value);
        }

        long value;
    };

    /**
     * template param<long long>
     */
    template <>
    struct CParam<long long>
    {
        CParam()
        {
            this->value = 0;
        }

        CParam(long long value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = stoll(str);
        }

        const string ToString()
        {
            return to_string(this->value);
        }

        long long value;
    };

    /**
     * template param<unsigned long>
     */
    template <>
    struct CParam<unsigned long>
    {
        CParam()
        {
            this->value = 0;
        }

        CParam(unsigned long value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = stoul(str);
        }

        const string ToString()
        {
            return to_string(this->value);
        }

        unsigned long value;
    };

    /**
     * template param<unsigned long long>
     */
    template <>
    struct CParam<unsigned long long >
    {
        CParam()
        {
            this->value = 0;
        }

        CParam(unsigned long long value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = stoull(str);
        }

        const string ToString()
        {
            return to_string(this->value);
        }

        unsigned long long value;
    };

    /**
     * template param<float>
     */
    template <>
    struct CParam<float>
    {
        CParam()
        {
            this->value = 0.0f;
        }

        CParam(float value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = stof(str);
        }

        const string ToString()
        {
            return to_string(this->value);
        }

        float value;
    };

    /**
    * template param<double>
    */
    template <>
    struct CParam<double>
    {
        CParam()
        {
            this->value = 0.0f;
        }

        CParam(double value)
        {
            this->value = value;
        }

        void FromString(const string &str)
        {
            this->value = stod(str);
        }

        const string ToString()
        {
            return to_string(this->value);
        }

        double value;
    };
}
#endif //EXHIREDIS_PARAM_H
