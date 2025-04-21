#pragma once

#include <memory>
#include <string>

class LargeClassImpl;
class LargeClass
{
    std::unique_ptr<LargeClassImpl> mImpl;

public:
    LargeClass();

    LargeClass(const LargeClass &other);
    LargeClass &operator=(const LargeClass &other);

    LargeClass(LargeClass &&other);
    LargeClass &operator=(LargeClass &&other);

    ~LargeClass();
    std::string GetString();
    int GetInt();
    double GetDouble();
};
