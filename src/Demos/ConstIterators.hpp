#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <vector>
#include <algorithm>

#include <iostream>

template <typename Type>
auto custom_cbegin(const Type &container) -> decltype(std::begin(container))
{
    return std::begin(container);
}

inline void OldIteratorsDemo()
{
    LOG_START_FUNCTION();

    std::vector<int> values;

    std::vector<int>::iterator it = std::find(values.begin(), values.end(), 1983);
    values.insert(it, 1998);

    std::cout <<
        R"(std::vector<int> values;
std::vector<int>::iterator it = std::find(values.begin(),values.end(),1983); 
values.insert(it,1998);

Wrong use of iterator, data are not changed here /|\ )"
              << std::endl;

    std::cout <<
        R"(
Old way (C++98):
typedef std::vector<int>::iterator IterT;
typedef std::vector<int>::const_iterator ConstIteratorT;

//This wont even compile now
ConstIteratorT const_iter = std::find(static_cast<ConstIteratorT>(values.begin()),
                                      static_cast<ConstIteratorT>(values.end()),
                                      1983);

values.insert(static_cast<IterT>(const_iter), 1998);
    )" << std::endl;

    auto it_new = std::find(values.cbegin(), values.cend(), 1983);
    values.insert(it_new, 1998);

    std::cout << R"(

New way: (C++11)
auto it_new = std::find(values.cbegin(), values.cend(), 1983);
values.insert(it_new, 19998);

Custom cbegin:
template <typename Type>
auto custom_cbegin(const Type &container) -> decltype(std::begin(container))
{
    return std::begin(container);
}

    )";

    auto begin = custom_cbegin(values);
    values.insert(begin, 1998);

    LOG_END_FUNCTION();
}

class ConstIteratorsDemo : public DemoBase
{
public:
    ConstIteratorsDemo()
    {
        mName = "ConstIteratorsDemo";
        mNotes = {};
    };
    ~ConstIteratorsDemo() = default;

    void ShowExample() override
    {
        PrintNotes();
        OldIteratorsDemo();
    }

    void
    ShowDemo() override
    {
        ShowExample();
    };
};
