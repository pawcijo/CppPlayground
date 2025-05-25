#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>

class BaseClass
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

inline void VirtualDestructorExample()
{
    LOG_START_FUNCTION();
    BaseClass *deriverd = new DerivedClass();

    deriverd->Use();

    delete dynamic_cast<DerivedClass *>(deriverd);

    LOG_END_FUNCTION();
}

class VirtualDestructorDemo : public DemoBase
{
public:
    VirtualDestructorDemo()
    {
        mName = "VirtualDestructorDemo";
        mNotes = {};
    };
    ~VirtualDestructorDemo() = default;

    void ShowExample(void(*printNotesCallback)(NoteFormat& notes) = nullptr) override
    {
          if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);;
        }
        VirtualDestructorExample();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
