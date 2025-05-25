#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <iostream>
#include <utility>
#include <vector>

namespace CompilationProcess
{
inline NoteFormat Notes = {
  { { "Preprocessing: " },
    { "\n* Expands macros (#define) \n* Inserts the contents of header files "
      "with #include.\n"
      "* Handles conditional compilation (#ifdef, #if). \n* Strips comments "
      "from the code.\n* Creates compilation unit from cpp file" } },
  { { "Compilation(Diagnostic - front end ): " },
    { "\nSyntax and semantic check(consistency of return types for functions,\n"
      "correctness of user-defined types, validity of type casting,type "
      "promotions,consistency of expressions with class interfaces)." } },
  { { "Compilation (Optimization - middle end): " },
    { "Abstract Syntax Tree creation and optimalization added depending on "
      "compilation flags." } },
  { { "Compilation: (Assembling): " },
    { "Generates assembly code from last step(language understood by "
      "processor)." } },
  { { "Object file creation: " },
    { "Translates assembly code into machine code, generating object "
      "files." } },
  { { "Linking: " },
    { "Combines object files and libraries into a final executable." } }
};

inline NoteFormat Summary = {
  { { "Process start: " }, { "Creating compilation units from cpp files." } },
  { { "Preprocessing: " },
    { "Expands macros, includes headers, and prepares the code for "
      "compilation." } },
  { { "Compilation (front-end middle-end  ): " },
    { "Converts C++ code into assembly code." } },
  { { "Assembling: " }, { "Translates assembly code generation." } },
  { { "Object file creation: " },
    { "Object files generation (.o or .obj), which contain machine code but "
      "are not yet linked together." } },
  { { "Linking: " },
    { "Combines object files and libraries into a final executable." } }
};

inline void Print(const NoteFormat& notes)
{
  for (const auto& pair : notes)
  {
    std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR
              << pair.second << std::endl;
  }
}
}

inline void CompilationProcessExplained()
{
  CompilationProcess::Print(CompilationProcess::Notes);
  std::cout << "\n--------------SUMMARY------------------------" << std::endl;
  CompilationProcess::Print(CompilationProcess::Summary);
}

class CompilationProcessDemo : public DemoBase
{
public:
  CompilationProcessDemo()
  {
    mName = "CompilationProcessDemo";
    mNotes = {};
  };
  ~CompilationProcessDemo() = default;

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
    CompilationProcessExplained();
  }

  void ShowDemo(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  };
};
