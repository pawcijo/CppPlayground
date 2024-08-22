#pragma once

#include "Common.hpp"

#include <iostream>

void ThrowStyleDemo()
{
    LOG_START_FUNCTION();

    std::cout << R"(
How to show that function should not throw any exceptions ?

// Old style C++98 less optimal (and also deprecated from c++17)
int func(int x) throw();
//If it throw std::unexpected() is called and terminate

// C++11 style
int func(int x) noexcept;
//If thrown std::terminate will be called)"
              << std::endl;

    LOG_END_FUNCTION();
}

void ThrowAndNoExceptDemo()
{
    ThrowStyleDemo();
}