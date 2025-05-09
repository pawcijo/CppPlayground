#pragma once

#include <iostream>
#include <ostream>

#include "Common.hpp"

class DemoBase
{
protected:
  std::string mName;
  NoteFormat mNotes;

public:
  DemoBase()
  {
    object_counter++;
    // Uncomment to see the number of objects created
    /*
    std::cout<<"...................Base type construcor....................\n";
    std::cout<<'\t'<<'\t'<<'\t'<< "Number of objects: " << object_counter <<
    "\n";
    std::cout<<"...........................................................\n";
    */
  }
  virtual ~DemoBase()
  {
    object_counter--;

    // Uncomment to see the number of objects destroyed
    /*
        std::cout<<std::endl<<std::endl;
        std::cout<<"...................Base type
       destructor....................\n"; std::cout<<'\t'<<'\t'<<'\t'<< "Number
       of objects : " << object_counter << "\n";
        std::cout<<"...........................................................\n";
        */
  };

  /*
  Print name of the demo.
  */
  virtual void PrintName()
  {
    std::cout << "\033[1;35m" << mName << "\033[0m "
              << ": \n"
              << std::endl;
  }

  virtual void PrintNotes()
  {
    PrintName();

    for (const auto& pair : mNotes)
    {
      std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR
                << std::endl
                << pair.second << std::endl
                << std::endl;
    }
  }

  virtual void ShowExample() = 0;

  virtual void ShowDemo() { ShowExample(); };

public:
  static long long object_counter;
};
