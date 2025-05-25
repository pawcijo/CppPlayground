#pragma once

#include <iostream>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

class FactoryTestObject
{
    std::string someString;

   public:
    FactoryTestObject() : someString("ExampleString")
    {
        std::cout << "Random obj created." << std::endl;
    };

    FactoryTestObject(const FactoryTestObject &another) = default;

    FactoryTestObject operator=(const FactoryTestObject &another)
    {
        someString = another.someString;

        return *this;
    }

    FactoryTestObject(const FactoryTestObject &&another)
    {
        someString = another.someString;
    };

    FactoryTestObject operator=(const FactoryTestObject &&another)
    {
        someString = another.someString;

        return *this;
    }

    std::string stringValue() const
    {
        return someString;
    }
};

inline void RandomLogger(const FactoryTestObject *ob)
{
    std::cout << ob->stringValue() << "(object destruction)" << std::endl;
}

class FactoryTestObjectFactory
{
   public:
    static auto CreateFactoryTestObject()
    {
        auto customDel = [](FactoryTestObject *obj)  // this is now
        {                                            // inside
            RandomLogger(obj);                       // makedelete
            delete obj;                              // Investment
        };
        std::unique_ptr<FactoryTestObject, decltype(customDel)> obj(nullptr, customDel);

        obj.reset(new FactoryTestObject());

        auto deleter = obj.get_deleter();
        deleter(obj.get());

        obj.release();

        return obj;
    }
};

inline void UniquePtrFactoryExample()
{
    LOG_START_FUNCTION();

    auto obj = FactoryTestObjectFactory::CreateFactoryTestObject();

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
        UniquePtrFactoryExample();
    }

    void ShowDemo() override
    {
        ShowExample();
    }
};
