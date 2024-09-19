#pragma once

#include "Common/Common.hpp"

#include <array>
#include <iostream>

#include "Common/DemoBase.hpp"

constexpr const char *expr = "dupa";
constexpr std::string_view kekw = "xD";

constexpr std::string CoDoChuja(const char *strv)
{
    return std::string(strv);
}

class Point
{

    std::array<double, 2> values; // x and y

public:
    constexpr Point(double a_X, double a_Y) : values()
    {
        values[0] = a_X;
        values[1] = a_Y;
    }
    constexpr Point(std::initializer_list<double> aList) : values()
    {
        int count = 0;
        for (double value : aList)
        {
            values[count] = value;
            count++;
        }
    }
    constexpr Point(Point &another) : values()
    {
        values[0] = another.values[0];
        values[1] = another.values[1];
    }

    constexpr Point &operator=(const Point &another)
    {
        values[0] = another.values[0];
        values[1] = another.values[1];
        return *this;
    }

    constexpr Point &operator=(std::initializer_list<double> aList)
    {
        int count = 0;
        for (double value : aList)
        {
            values[count] = value;
            count++;
        }
        return *this;
    }

    Point() = delete;
};

constexpr auto value = 5;
constexpr auto value_2 = 5;

constexpr auto lista = {2.0, 1.0};
constexpr Point point = lista;

void ConstExpressionExample()
{
    LOG_START_FUNCTION();

    constexpr auto arraySize2 = 10;
    std::array<int, arraySize2> data2;

    std::string xd = expr;

    std::string expr_2 = "dupa_2";

    std::cout << CoDoChuja(expr_2.c_str()) << std::endl;

    std::cout << "Array size: " << data2.size() << std::endl;

    LOG_END_FUNCTION();
}


class ConstExpressionDemo : public DemoBase
{
public:
    ConstExpressionDemo()
    {
        mName = "ConstExpressionDemo";
        mNotes = {};
    };
    ~ConstExpressionDemo() = default;

    void ShowExample() override
    {
        PrintNotes();
        ConstExpressionExample();
    }

    void ShowDemo() override
    {
        ShowExample();
    };
};
