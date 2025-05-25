#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

namespace UniversalRefVsRValueRef
{

}

class UniversalRefVsRValueRefDemo : public DemoBase
{
   public:
    UniversalRefVsRValueRefDemo()
    {
        mName = "UniversalRefVsRValueRefDemo";
        mNotes = {{{"Universal reference examples:"}, {R"(template<typename T>
void f(T&& param); // param is a universal reference

auto&& var2 = var1; // var2 is a universal reference
)"}},{{R"(What these contexts have in common is the presence of type deduction. In the template
f, the type of param is being deduced, and in the declaration for var2, var2’s
type is being deduced. Compare that with the following examples (also from the sample
code above), where type deduction is missing. If you see “T&&” without type
deduction, you’re looking at an rvalue reference:)"},{"\n source - Effective Modern c++ 165 "}},
                  {{"Without deduction:"}, 
                  {
R"(void f(Widget&& param); // no type deduction;
                        // param is an rvalue reference

Widget&& var1 = Widget(); // no type deduction;
                          // var1 is an rvalue reference
)"}}};
    }
    void ShowExample(void(*printNotesCallback)(NoteFormat& notes) = nullptr) override
    {
        if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);
        }
    }

    void ShowDemo() override
    {
        ShowExample();
    }
};