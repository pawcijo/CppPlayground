#pragma once

#include "Common/Common.hpp"

#include <iostream>
#include <memory>

namespace HowToWeakPtr
{

    class TestObject
    {
        std::string someString;

    public:
        TestObject() : someString("ExampleString")
        {
            std::cout << "Test obj created." << std::endl;
        };

        TestObject(const TestObject &another) = default;

        TestObject operator=(const TestObject &another)
        {
            someString = another.someString;

            return *this;
        }

        TestObject(const TestObject &&another)
        {
            someString = another.someString;
        };

        TestObject operator=(const TestObject &&another)
        {
            someString = another.someString;

            return *this;
        }

        std::string stringValue() const { return someString; }
    };

    void HowToWeakPtrExample()
    {
        LOG_START_FUNCTION();

        auto obj = std::make_shared<TestObject>();

        auto weakObj = std::weak_ptr<TestObject>(obj);

        auto weakObjTest = weakObj.lock();

        if (!weakObj.expired())
        {
            std::cout << "Obj still exist, use count: " << weakObj.use_count() << "\n"
                      << std::endl;
        }

        obj.reset();

        if (!weakObj.expired())
        {
            std::cout << "Obj still exist, use count: " << weakObj.use_count() << "\n"
                      << std::endl;
        }
        else
        {
            std::cout << "Obj does not exist \n"
                      << std::endl;
        }

        weakObjTest.reset();

        if (!weakObj.expired())
        {
            std::cout << "Obj still exist, use count: " << weakObj.use_count() << "\n"
                      << std::endl;
        }
        else
        {
            std::cout << "Obj does not exist \n"
                      << std::endl;
        }

        LOG_END_FUNCTION();
    }
}

class HowToWeakPtrDemo : public DemoBase
{
public:
    HowToWeakPtrDemo()
    {
        mName = "HowToWeakPtrDemo";
        mNotes = {};
    };
    ~HowToWeakPtrDemo() = default;

    void ShowExample() override
    {
        PrintNotes();
        HowToWeakPtr::HowToWeakPtrExample();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
