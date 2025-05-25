#pragma once

#include <iostream>
#include "Common/Common.hpp"

#include "Common/DemoBase.hpp"

using namespace std;

// decltype
template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u)
{
    return t + u;
}

// universal reference address

template <typename T>
void printAddress(const char *name, T &&param)
{
    printf("%s : Address of universal ref param: %p\n", name,
           (void *)(&param));
}

// copy address type
template <typename T>
void printAddressCopy(const char *name, T param)
{
    printf("%s : Address of param: %p\n",
           name, static_cast<void *>(&param));
}

inline void UniveralTypeDemo()
{
    LOG_START_FUNCTION();
    int x = 25;
    int y = x;
    int &z = x;

    printAddress("25", 25);              // Some address A
    printAddress("int x  (x)", x);       // Some address B
    printAddress("int y = x;", y);       // Some Address C
    printAddress("int &z = x  (z)", z);  // B ( z is ref of x)

    LOG_END_FUNCTION();
}

inline void DecltypeDemo()
{
    LOG_START_FUNCTION();
    int x = 3;
    float y = 0.5f;

    // compile time type deduction
    if (typeid(float) == typeid(add(x, y)))
    {
        cout << "Deducted type float.\n";
    }
    cout << add(x, y) << endl;
    LOG_END_FUNCTION();
}

inline void PtrTypeTemplateDeductionDemo()
{

    LOG_START_FUNCTION();

    const char *const dupaString = "Dupa";

    printf("const char *const : Address of param: %p\n",
           (void *)dupaString); // Some address A

    printAddressCopy("const char *const", dupaString); // Some address B (copy new address)
    printAddress("const char *const", dupaString);     // Some address C (content of the type is const
                                                       // but ptr itself is lost)
    LOG_END_FUNCTION();
}

template <typename T>
void ShowParamTypeNameByCopy(const char *passedType, T param)
{
    printf("- - - - - - - ShowParamTypeNameByCopy - - - - - - - \n");
    printf("Passed type: %s , param type : %s \n", passedType,
           typeid(param).name());
}

template <typename T>
void ShowParamTypeNameByRef(const char *passedType, T &param)
{
    printf("- - - - - - - ShowParamTypeNameByRef - - - - - - - \n");

    printf("Passed type: %s , param type : %s \n", passedType,
           typeid(param).name());
}

template <typename T, std::size_t N>               // see info
constexpr std::size_t arraySize(T (&)[N]) noexcept // below on
{
    return N; // and
}

inline void PtrVsArrayDemo()
{
    LOG_START_FUNCTION();
    const char name[] = "J. P. Briggs";
    const char *ptrToName = name;

    if (typeid(name) != typeid(ptrToName))
    {
        printf("This types are not the same \n");
        printf("name type : %s \n", typeid(name).name());
        printf("ptrToName type : %s \n", typeid(ptrToName).name());
    }
    printf("\n");

    ShowParamTypeNameByCopy("const char name[]", name);
    ShowParamTypeNameByCopy("const char *ptrToName", ptrToName);

    printf("\n");

    ShowParamTypeNameByRef("const char name[]", name);
    ShowParamTypeNameByRef("const char *ptrToName", ptrToName);
    printf("\n");

    printf("Compile time array size : %zu \n", arraySize(name));
    LOG_END_FUNCTION();
}

class DeductingTypesDemo : public DemoBase
{
    public:
    DeductingTypesDemo()
    {
        mName = "DeductingTypesDemo";
        mNotes = {};
    };
    ~DeductingTypesDemo() = default;

    void PrintNotes() override
    {
        PrintName();
        for (const auto &pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample() override
    {
        PrintNotes();
        UniveralTypeDemo();
        DecltypeDemo();
        PtrTypeTemplateDeductionDemo();
        PtrVsArrayDemo();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
