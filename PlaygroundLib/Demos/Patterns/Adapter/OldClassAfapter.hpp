#pragma once

#include <iostream>
#include <memory>
#include "MyOldClass.hpp"
#include "ClientInterface.hpp"

class OldClassAdapter: public ClientInterface
{
public:
    OldClassAdapter(std::unique_ptr<MyOldClass> oldClass) : mOldClass(std::move(oldClass)) {}
    ~OldClassAdapter() = default;

    void doSomethingCool() override
    {
        mOldClass->doSomething();
    }

    void doSomething() override
    {
        std::cout << "Doing something cool!" << std::endl;
    }

    void doSomethingElse() override
    {
        mOldClass->doSomethingElse();
    }

private:
    std::unique_ptr<MyOldClass> mOldClass;
};
