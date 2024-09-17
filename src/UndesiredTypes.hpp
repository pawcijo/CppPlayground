#pragma once

#include <vector>
#include <iostream>


#include "Common/Common.hpp"

class Widget
{
    int mValue;

public:
    Widget(int aValue) : mValue(aValue) {};
    Widget() { mValue = 0; }

    Widget* operator=(const Widget &){
        Widget * widget = new Widget();
        return widget;
    }

    int &Value() { return mValue; }
};

std::vector<bool> features(const Widget & /*w*/)
{
    std::vector<bool> featuresVec;

    for (int i = 0; i < 5; i++)
    {
        featuresVec.push_back(i);
    }
    return featuresVec;
}

void processWidget(const Widget& /*w*/, bool priority)
{
    std::cout<<"Priosirty"<<priority<<std::endl;
}

void AutoDeducesWrongDemo()
{
    LOG_START_FUNCTION();

    Widget widget;

    bool something = features(widget)[5];
    auto somethingAuto =  features(widget)[5];
    auto highPriorityExplicit = static_cast<bool>(features(widget)[5]);

    detectValueCategoryImpl(something);
    detectValueCategoryImpl(somethingAuto);
    detectValueCategoryImpl(highPriorityExplicit);
    detectValueCategoryImpl(features(widget)[5]);

    processWidget(widget,something);
    processWidget(widget,somethingAuto);


    LOG_END_FUNCTION();
}

void UndesiredTypesDemo()
{

    AutoDeducesWrongDemo();
}
