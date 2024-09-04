#pragma once

#include "Common.hpp"

#include <iostream>

namespace RuleOfThree
{
    static const char *Note = R"(
The Rule of Three
states that if you declare any of a copy constructor, copy assignment operator, or
destructor, you should declare all three.
)";


void ExampleDemo()
{
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

void RuleOfThreeDemo()
{
    std::cout << Note << std::endl;
    ExampleDemo();
}
}