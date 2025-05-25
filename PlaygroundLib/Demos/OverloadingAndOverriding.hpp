#pragma once
#include <iostream>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

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

inline void OverloadingRvalueLvalue()
{
    LOG_START_FUNCTION();
    WidgetTest().doWork();

    WidgetTest aWidget;
    aWidget.doWork();

    LOG_END_FUNCTION();
}

class OverloadingAndOverridingDemo : public DemoBase
{
   public:
    OverloadingAndOverridingDemo()
    {
        mName = "OverloadingAndOverridingDemo";
        mNotes = {};
    };
    ~OverloadingAndOverridingDemo() = default;

    void ShowExample(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
    {
    if (nullptr == printNotesCallback)
    {
      PrintNotes();
    }
    else
    {
      printNotesCallback(mNotes);
    }
        OverloadingRvalueLvalue();
    }

    void ShowDemo(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
    {
        ShowExample(printNotesCallback);
    };
};
