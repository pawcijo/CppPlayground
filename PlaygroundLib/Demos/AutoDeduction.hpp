#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

template <typename T>
void func_for_T(const char *VariableSignarute, T param)
{

    printf("[T param],  param type of variable[%s]: %s \n", VariableSignarute,
           typeid(param).name());
}

template <typename T>
void func_for_InitalizerListOfT(const char *VariableSignarute, std::initializer_list<T> param)
{

    printf("[T param],  param type of variable[%s]: %s \n", VariableSignarute,
           typeid(param).name());
}

template <typename T>
void func_for_ConstT(const char *VariableSignarute, const T param)
{
    printf("[const T param], param type of variable[%s]: %s \n", VariableSignarute,
           typeid(param).name());
}

template <typename T>
void func_for_ConstRefT(const char *VariableSignarute, const T &param)
{
    printf("[const T& param], param type of variable[%s]: %s \n", VariableSignarute,
           typeid(param).name());
}

class AutoDeductionDemo : public DemoBase
{
public:
    AutoDeductionDemo()
    {
        mName = "AutoDeductionDemo";
        mNotes = {};
    };
    ~AutoDeductionDemo() = default;

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
        LOG_START_FUNCTION();
    
        func_for_T("27", 27);
        detectValueCategory(27);
        std::cout << std::endl;

        auto x = 27;
        func_for_ConstRefT("auto x = 27;", x);
        detectValueCategory(x);
        std::cout << std::endl;

        const auto cx = x;
        func_for_ConstT("const auto cx = x;", cx);
        detectValueCategory(cx);
        std::cout << std::endl;

        auto &&uref1 = x;
        func_for_T(" auto &&uref1 = x;", uref1);
        detectValueCategory(uref1);
        std::cout << std::endl;

        auto &&uref2 = cx;
        func_for_ConstRefT("auto &&uref2 = cx;", uref2);
        detectValueCategory(uref2);
        std::cout << std::endl;

        auto &&uref3 = 27;
        func_for_ConstT("auto &&uref3 = 27;", uref3);
        detectValueCategory(uref3);
        std::cout << std::endl;

        const char name[] = // name's type is const char[13]
            "R. N. Briggs";

        func_for_ConstT(" const char name[] = ...;", name);
        func_for_T(" const char name[] = ...;", name);
        func_for_ConstRefT(" const char name[] = ...;", name);
        detectValueCategory(name);
        std::cout << std::endl;

        auto arr1 = name;

        func_for_ConstT("auto arr1 = name;", arr1);
        detectValueCategory(arr1);
        std::cout << std::endl;

        auto &arr2 = name;

        func_for_ConstT("auto &arr2 = name;", arr2);
        detectValueCategory(arr2);
        std::cout << std::endl;

        std::cout << "- - - - - Initializer list vs normal - - - - - " << std::endl;

        auto x1 = 27;
        func_for_T("auto x1 = 27;", x1);
        detectValueCategory(x1);
        std::cout << std::endl;

        auto x2(27);
        func_for_T("auto x2(27);", x2);
        detectValueCategory(x2);
        std::cout << std::endl;

        auto x3 = {27};
        func_for_T("auto x3 = {27};", x3);
        detectValueCategory(x3);
        std::cout << std::endl;

        auto x4{27};
        func_for_T("auto x4{27};", x4);
        func_for_ConstRefT("auto x4{27};", x4);
        func_for_ConstT("auto x4{27};", x4);

        printf("[T param],  param type of variable[%s]: %s \n", "auto x4{27};",
               typeid(x4).name());

        detectValueCategory(x4);
        std::cout << std::endl;

        // This will not work, type deduciton will fail
        // auto x5 = { 1, 2, 3.0 };

        auto x_list = {11, 23, 9};
        func_for_T(" auto x_list = { 11, 23, 9 };", x_list);

        // This will not work
        // func_for_T(" auto x_list = { 11, 23, 9 };",{ 11, 23, 9 } );
        func_for_InitalizerListOfT("{ 11, 23, 9 }", {11, 23, 9});

        LOG_END_FUNCTION();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
