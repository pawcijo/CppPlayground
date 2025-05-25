#pragma once

#include <algorithm>
#include <cstdint>
#include <execution>
#include <vector>

#include "Common/DemoBase.hpp"

class ExecutionDemo : public DemoBase
{
   public:
    ExecutionDemo()
    {
        mName = "ExecutionDemo";
        mNotes = {
            {"<execution>",
             R"(The <execution> header in the C++ Standard Library provides support for parallel algorithms. 
It introduces execution policies that allow you to specify how certain 
standard algorithms should be executed, enabling parallel and vectorized execution.

        )"}};
    }

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
    }

    void ShowDemo(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
    {
        ShowExample(printNotesCallback);
    }

   private:
    void BasicExample()
    {
        LOG_FUNCTION_NAME(CYAN);

        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        // exection polices  not supported on ARM Macos (clang 16.0.0)
#if defined(__cpp_lib_execution)

        // Sequential execution
        std::for_each(std::execution::seq, data.begin(), data.end(), [](int& n) { n *= 2; });

        // Parallel execution
        std::for_each(std::execution::par, data.begin(), data.end(), [](int& n) { n *= 2; });

        // Parallel and unsequenced execution
        std::for_each(std::execution::par_unseq, data.begin(), data.end(), [](int& n) { n *= 2; });

        for (const auto& n : data)
        {
            std::cout << n << " ";
        }
#else
        std::cout << "Execution policies are not supported (ARM Macos)." << std::endl;
#endif
    }
};