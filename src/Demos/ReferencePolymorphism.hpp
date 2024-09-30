#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>

namespace ReferencePolymorphism
{
static const char *Note = R"(
Polymorphism allows a base class reference to refer to objects of derived classes.
)";


class PureVirtualClass
{

public:
    PureVirtualClass()
    {
        std::cout << "PureVirtualClass class constructor \n";
    }

    ~PureVirtualClass()
    {
        std::cout << "PureVirtualClass class destructor\n";
    }

    virtual void Use() = 0;
};

class BaseClass : public PureVirtualClass
{

public:
    BaseClass()
    {
        std::cout << "Created Base class \n";
    }

    ~BaseClass()
    {
        std::cout << "Destroyed Base class \n";
    }

    virtual void Use()
    {
        std::cout << "Use Base class \n";
    }
};

class DerivedClass final : public BaseClass
{

public:
    DerivedClass()
    {
        std::cout << "Created  Derived class \n";
    }

    ~DerivedClass()
    {
        std::cout << "Destroyed  Derived class \n";
    }

    void Use() override
    {
        std::cout << "Use Derived class \n";
    }
};
}

inline void RefrencePoliDemo()
{
    LOG_START_FUNCTION();
    ReferencePolymorphism::BaseClass A;
    ReferencePolymorphism::DerivedClass B;

    ReferencePolymorphism::PureVirtualClass &refToA = A;
    ReferencePolymorphism::PureVirtualClass &refToB = B;

    refToA.Use();
    refToB.Use();

    LOG_END_FUNCTION();
}

class ReferencePolymorphismDemo : public DemoBase
{
public:
    ReferencePolymorphismDemo()
    {
        mName = "ReferencePolymorphismDemo";
        mNotes = {};
    };
    ~ReferencePolymorphismDemo() = default;

    void ShowExample() override
    {
        PrintNotes();

        std::cout << BOLD_TEXT_START << ReferencePolymorphism::Note << BOLD_TEXT_END << std::endl;
        RefrencePoliDemo();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
