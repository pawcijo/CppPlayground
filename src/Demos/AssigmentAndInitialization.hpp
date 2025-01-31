#pragma once

#include <iostream>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

class FancyWidget
{
    int mValue;

   public:
    FancyWidget(int aValue) : mValue(aValue)
    {
        std::cout << "Param construcotr" << std::endl;
    };
    FancyWidget()
    {
        std::cout << "Default construcotr" << std::endl;
        mValue = 0;
    }

    FancyWidget(const FancyWidget &)
    {
        std::cout << "Copy constructor" << std::endl;
    }

    FancyWidget &operator=(const FancyWidget &other)
    {
        std::cout << "Copy assgin constructor" << std::endl;

        if (this == &other)
        {
            return *this;  // Self-assignment check
        }
        mValue = other.mValue;
        return *this;
    }

    FancyWidget(FancyWidget &&other)
    {
        std::cout << "Move constructor" << std::endl;
        mValue = other.mValue;
        other.mValue = 0;  // clear previous
    }

    FancyWidget &operator=(FancyWidget &&other)
    {
        std::cout << "Move assgin constructor" << std::endl;

        if (this == &other)
        {
            return *this;  // Self-assignment check
        }
        mValue = other.mValue;
        other.mValue = 0;  // Leave 'other' in a valid state
        return *this;
    }

    int &Value()
    {
        return mValue;
    }
};

class AssigmentDemo : public DemoBase
{
   public:
    AssigmentDemo()
    {
        mName = "AssigmentDemo";
        mNotes = {};
    };
    ~AssigmentDemo() = default;

    void ShowExample() override
    {
        PrintNotes();
        AssigmentAndInitialization();
    }

    void ShowDemo() override
    {
        ShowExample();
    };

   private:
    void AssigmentAndInitialization()
    {
        LOG_START_FUNCTION();

        FancyWidget aFancy;
        FancyWidget bFancy = aFancy;
        FancyWidget cFancy(1);

        Show_address(aFancy);
        Show_address(bFancy);
        Show_address(cFancy);

        cFancy = aFancy;
        Show_address(cFancy);

        cFancy = std::move(aFancy);
        Show_address(cFancy);
        Show_address(aFancy);

        LOG_END_FUNCTION();
    }
};
