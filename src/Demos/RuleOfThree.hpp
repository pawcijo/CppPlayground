#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>

static const char *Note = R"(
The Rule of Three
states that if you declare any of a copy constructor, copy assignment operator, or
destructor, you should declare all three.
)";

inline void ExampleDemo()
{

    std::cout << Note << std::endl;
    LOG_START_FUNCTION();

    std::cout << R"(
class Example
{
    public:
    Example();

    Example(Example &);
    // Now you should also write your own
    // Example& operator=(const Example& example)
    //
    // and
    //
    //~Example();
};

)" << std::endl;

    LOG_END_FUNCTION();
}

class RuleOfThreeDemo : public DemoBase
{
public:
    RuleOfThreeDemo()
    {
        mName = "RuleOfThreeDemo";
        mNotes = {};
    };
    ~RuleOfThreeDemo() = default;

    void ShowExample() override
    {
        PrintNotes();
        ExampleDemo();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
