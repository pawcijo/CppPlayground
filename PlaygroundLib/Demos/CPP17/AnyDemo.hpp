#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

// This any
#include <any>

class AnyDemo : public DemoBase
{
    struct NonCopyable
    {
        NonCopyable()
        {
            std::cout << "NonCopyable type \n";
        };
        NonCopyable(const NonCopyable&) = delete;  // Copy constructor deleted
    };

    struct Copyable
    {
        Copyable()
        {
            std::cout << "Copyable type \n";
        }
        int value;
    };

   public:
    AnyDemo()
    {
        mName = "AnyDemo";
        mNotes = {{"std::any", R"(std::any allows you to store values of any type. 
It acts as a type-safe container that can hold different types of values and change them
at runtime without needing to specify a fixed type beforehand.)"}};
    };
    ~AnyDemo() = default;

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
        BasicUsage();
        CheckValue();
        CastingError();
        CopyableExample();
    }

    void ShowDemo() override
    {
        ShowExample();
    };

   private:
    void BasicUsage()
    {
        LOG_FUNCTION_NAME(YELLOW);

        std::any zmienna_tego_typu = 2137;
        std::cout << "Type: " << zmienna_tego_typu.type().name() << " \n"
                  << "Value: " << std::any_cast<int>(zmienna_tego_typu) << "\n";
    }

    // Checking if std::any Holds a Value
    void CheckValue()
    {
        LOG_FUNCTION_NAME(YELLOW);

        std::any var;

        if (!var.has_value())
        {
            std::cout << "var is empty\n";
        }

        var = 3.14;  // Assigning a value
        if (var.has_value())
        {
            std::cout << "var contains: " << std::any_cast<double>(var) << '\n';
        }
    }

    void CastingError()
    {
        LOG_FUNCTION_NAME(YELLOW);

        std::any var = 100;

        try
        {
            // Good cast
            std::cout << std::any_cast<int>(var) << '\n';
            // std::any_cast<T> does not perform implicit conversions
            std::cout << std::any_cast<double>(var) << '\n';  // Incorrect type casting
        }
        catch (const std::bad_any_cast& e)
        {
            std::cout << "Casting error: " << e.what() << '\n';
        }
    }

    void CopyableExample();
};

inline void AnyDemo::CopyableExample()
{
    LOG_FUNCTION_NAME(YELLOW);

    std::any a = Copyable{};

    std::cout << RED_BOLD_TEXT_START << "Try to remove comment in function and compile"
              << TEXT_COLOR_CLEAR " \n";

    // This will not compile!
    //  std::any b = NonCopyable{};
}
