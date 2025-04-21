#pragma once

#include <iostream>

class MyOldClass
{
public:
  ~MyOldClass() = default;

  void doSomething()
  {
    std::cout << "Doing something cool!" << std::endl;
  }
  void doSomethingElse()
  {
  }
};