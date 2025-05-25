#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include <deque>

#include <utility>
#include <string>

template <typename T, typename I>  // C++14;
auto authAndAccessAuto(T &&c, I i) // not quite
{                                  // correct
    // authenticateUser();
    std::cout << "Return type: \n";
    detectValueCategoryImpl(std::forward<T>(c)[i]);

    std::cout << "PassedType type: \n";
    detectValueCategoryImpl(std::forward<T>(c));
    return c[i]; // return type deduced from c[i]
}

template <typename T, typename I>    // C++14; works,
auto                                 // but still
authAndAccessDeclareType(T &&c, I i) // requires
    -> decltype(std::forward<T>(c)[i])
{
    std::cout << "Return type: \n";
    detectValueCategoryImpl(std::forward<T>(c)[i]);

    std::cout << "PassedType type: \n";
    detectValueCategoryImpl(std::forward<T>(c));
    return std::forward<T>(c)[i];
}

inline auto makeStringDeque()
{
    return std::deque<std::string>{
        std::string("dupa"),
        std::string("babyn"),
        std::string("ło"),
        std::string("tego"),
        std::string("rypu")};
}

inline void DeclareTypeVsAutoDemo()
{
    LOG_START_FUNCTION();

    std::deque<int> d;

    for (int i = 0; i < 5; i++)
    {
        d.push_back(i);
    }

    // authAndAccessAuto(d,5) = 10; // this will not compile
    authAndAccessAuto(d, 5);
    authAndAccessDeclareType(d, 5) = 6;

    auto s = authAndAccessAuto(makeStringDeque(), 5);
    auto s2 = authAndAccessDeclareType(makeStringDeque(), 5);
    auto s3 = authAndAccessDeclareType(std::deque<std::string>{
                                           std::string("dupa"),
                                           std::string("babyn"),
                                           std::string("ło"),
                                           std::string("tego"),
                                           std::string("rypu")},
                                       5);

    LOG_END_FUNCTION();
}

inline void DeclareTypeVsAutoVariableDemo()
{
    LOG_START_FUNCTION();

    int x = 5;
    const int &cx = x;

    auto cy = x;
    decltype(auto) cz = cx;

    detectValueCategoryImpl(5);
    detectValueCategoryImpl(x);
    detectValueCategoryImpl(cy);
    detectValueCategoryImpl(cz);

    LOG_END_FUNCTION();
}

inline decltype(auto) f1()
{
    int x = 0;
    return x; // decltype(x) is int, so f1 returns int
}

/* Thats not good
decltype(auto) f2()
{
    int x = 0;
    return (x); // decltype((x)) is int&, so f2 returns int&
}
*/

inline void ParethisisDiffForDecltypeDemo()
{
    LOG_START_FUNCTION();
    detectValueCategoryImpl(f1());
    // detectValueCategoryImpl(f2());

    LOG_END_FUNCTION();
}

template <typename T> // declaration only for TD;
class TD;

inline void TemplateTest()
{
    LOG_START_FUNCTION();

    // const int theAnswer = 42;
    // auto x = theAnswer;
    // auto y = &theAnswer;

    // TD<decltype(x)> xType; // elicit errors containing
    // TD<decltype(y)> yType; // x's and y's types

    LOG_END_FUNCTION();
}

class DeclareTypeDemo : public DemoBase
{
public:
    DeclareTypeDemo()
    {
        mName = "DeclareTypeDemo";
        mNotes = {};
    };
    ~DeclareTypeDemo() = default;

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
        DeclareTypeVsAutoDemo();
        DeclareTypeVsAutoVariableDemo();
        ParethisisDiffForDecltypeDemo();
    }

    void ShowDemo(void(*printNotesCallback)(NoteFormat& notes) = nullptr) override
    {
        ShowExample(printNotesCallback);
    };
};