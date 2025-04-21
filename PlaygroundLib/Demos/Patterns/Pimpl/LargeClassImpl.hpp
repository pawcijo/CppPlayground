#pragma once

#include <string>

class LargeClassImpl
{
    std::string someString;
    int someInt;
    double someDouble;

public:
    LargeClassImpl() : someString({}), someInt(0), someDouble(0.0) {}
    std::string GetString();
    int GetInt();
    double GetDouble();
};