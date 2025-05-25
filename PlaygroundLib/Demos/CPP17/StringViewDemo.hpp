#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

// This string_view
#include <string_view>

class StringViewDemo : public DemoBase
{
   public:
    StringViewDemo()
    {
        mName = "StringViewDemo";
        mNotes = {
            {"std::string_view:",
             R"(std::string_view is a lightweight, non-owning view of a sequence of characters, 
introduced in C++17. It allows you to refer to a string (such as a std::string or 
a C-style string) without making a copy, improving performance in many scenarios.)"}};
    };
    ~StringViewDemo() = default;

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
        BasicExample();
        StringViewOperationsExample();
    }

    void ShowDemo(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
    {
        ShowExample(printNotesCallback);
    };

   private:
    void printLine(std::string_view sv)
    {
        std::cout << "String view: " << sv << "\n";
    }

    void BasicExample()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::string str = "Hello, World!";
        printLine(str);           // Works with std::string
        printLine("Hello, C++");  // Works with string literals
    }

    void StringViewOperationsExample()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::string str = "Hello, World!";
        std::string_view sv = str;

        // Access the first character
        std::cout << "First character: " << sv.front() << "\n";

        // Access the last character
        std::cout << "Last character: " << sv.back() << "\n";

        // Access the nth character
        std::cout << "Character at index 7: " << sv[7] << "\n";

        // Get the length
        std::cout << "Length: " << sv.length() << "\n";

        // Check if empty
        std::cout << "Empty: " << std::boolalpha << sv.empty() << "\n";

        // Remove the first character
        sv.remove_prefix(1);
        std::cout << "After removing the first character: " << sv << "\n";

        // Remove the last character
        sv.remove_suffix(1);
        std::cout << "After removing the last character: " << sv << "\n";

    }
};
