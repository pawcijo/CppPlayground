#pragma once

#include "Common.hpp"
#include <iostream>

class MutableConst
{
    int variable;
    mutable int variable_v2;

public:
    MutableConst(int aV) : variable(aV), variable_v2(0) {}

    void FunnyFunction() const
    {
        variable_v2 = 2137;
        std::cout << "FunnFunction variable:" << variable << std::endl;
        std::cout << "FunnFunction variable_v2:" << variable_v2 << std::endl;
    }
};

void ShowMutableconst()
{
    LOG_START_FUNCTION();

    std::cout << R"(

class MutableConst
{
    int variable;
    mutable int variable_v2;

public:
    MutableConst(int aV) : variable(aV), variable_v2(0) {}

    void FunnyFunction() const
    {
             variable_v2 = 2137;
        std::cout<<"FunnyFunction variable:"<<variable<<std::endl;
        std::cout<<"FunnyFunction variable_v2:"<<variable_v2<<std::endl;
    }
};

MutableConst aMC(6);
aMC.FunnyFunction();

)";

    MutableConst aMC(6);
    aMC.FunnyFunction();

    LOG_END_FUNCTION();
}

void ConstMutableDemo()
{

    ShowMutableconst();
}