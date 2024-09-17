#pragma once
#include "Common/Common.hpp"

#include <iostream>

class WidgetTest
{
public:
    // this version of doWork applies
    // only when *this is an lvalue
    void doWork() &
    {
        std::cout << "This is done by lvalue \n";
    }
    // this version of doWork applies
    // only when *this is an rvalue
    void doWork() &&
    {
        std::cout << "This is done by rvalue \n";
    }
};

void OverloadingRvalueLvalue()
{
    LOG_START_FUNCTION();
    WidgetTest().doWork();

    WidgetTest aWidget;
    aWidget.doWork();
    

    LOG_END_FUNCTION();
}

void OverloadingAndOverridingDemo()
{
    OverloadingRvalueLvalue();
}