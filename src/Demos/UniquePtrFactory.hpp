#pragma once

#include <iostream>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

namespace UniquePtrFactory
{

class TestObject
{
    std::string someString;

   public:
    TestObject() : someString("ExampleString")
    {
        std::cout << "Random obj created." << std::endl;
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

    std::string stringValue() const
    {
        return someString;
    }
};

inline void RandomLogger(const TestObject *ob)
{
    std::cout << ob->stringValue() << "(object destruction)" << std::endl;
}

class TestObjectFactory
{
   public:
    static auto CreateTestObject()
    {
        auto customDel = [](TestObject *obj)  // this is now
        {                                     // inside
            RandomLogger(obj);                // makedelete
            delete obj;                       // Investment
        };
        std::unique_ptr<TestObject, decltype(customDel)> obj(nullptr, customDel);

        obj.reset(new TestObject());

        auto deleter = obj.get_deleter();
        deleter(obj.get());

        obj.release();

        return obj;
    }
};

inline void UniquePtrFactoryExample()
{
    LOG_START_FUNCTION();

    auto obj = TestObjectFactory::CreateTestObject();

    if (nullptr == obj)
    {
        std::cout << "Empty obj" << std::endl;
    }

    LOG_END_FUNCTION();
}

class UniquePtrFactoryDemo : public DemoBase
{
   public:
    UniquePtrFactoryDemo()
    {
        mName = "UniquePtrFactoryDemo";
        // TODO write some notes :)
        mNotes = {{{"TODO write some notes"}, {}}};
    }
    void ShowDemo() override
    {
        PrintNotes();
        UniquePtrFactoryExample();
    }
};

}  // namespace UniquePtrFactory