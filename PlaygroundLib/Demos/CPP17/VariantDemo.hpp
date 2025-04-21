#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

// This variant
#include <variant>

// TODO
class VariantDemo : public DemoBase
{
    union UnsafeUnion
    {
        int i;
        double d;
        char str[32];  // Fixed-size buffer for string

        UnsafeUnion()
        {
            std::memset(this, 0, sizeof(*this));
        }  // Ensure initialization
    };

   public:
    VariantDemo()
    {
        mName = "VariantDemo";
        mNotes = {
            {"std::variant", R"(is a type-safe alternative to union in C++, introduced in C++17. 
It allows you to store one value out of multiple possible types, 
ensuring type safety by tracking the active type.)"}};
    };
    ~VariantDemo() = default;

    void PrintNotes() override
    {
        PrintName();

        for (const auto& pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample() override
    {
        PrintNotes();
        BasicExample();
        UnionVsVariant();
        VistUsage();
        EmptyVariant();
    }

    void ShowDemo() override
    {
        ShowExample();
    };

   private:
    void BasicExample()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::variant<int, double, std::string> myVariant;  // Can hold int, double, or std::string

        myVariant = 10;  // Holds an int
        std::cout << std::get<int>(myVariant) << '\n';

        myVariant = 3.14;  // Holds a double
        std::cout << std::get<double>(myVariant) << '\n';

        myVariant = "Hello";  // Holds a std::string
        std::cout << std::get<std::string>(myVariant) << '\n';

        // Get value passing type index
        std::cout << std::get<2>(myVariant) << "\n";

        // check if currently holds alternative type
        if (std::holds_alternative<double>(myVariant))
        {
            std::cout << "It holds a double!\n";
        }
        else
        {
            std::cout << "It holds a " << myVariant.index() << " !\n";
        }
    }

    void UnionVsVariant()
    {
        LOG_FUNCTION_NAME(YELLOW);
        UnsafeUnion u;
        u.i = 42;  // Works
        std::cout << u.i << "\n";

        u.d = 3.14;  // Overwrites previous data
        std::cout << u.d << "\n";

        std::strcpy(u.str, "Hello");  // Overwrites previous data
        std::cout << u.str << "\n";
    }

    void VistUsage()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::variant<int, double, std::string> myVariant;  // Can hold int, double, or std::string

        myVariant = 10;  // Holds an int

        std::visit([](auto&& value) { std::cout << value << '\n'; }, myVariant);
    }

    void EmptyVariant()
    {
        LOG_FUNCTION_NAME(YELLOW);

        std::variant<std::monostate, int, std::string> v;
        if (std::holds_alternative<std::monostate>(v))
        {
            std::cout << "Currently empty\n";
        }
    }
};
