#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include "Common/MyUniquePtr.hpp"
#include "Common/MySharedPtr.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

inline void MyUniquePtrDemo()
{
  LOG_START_FUNCTION();

  class TmpClass
  {
    int mInt;

  public:
    TmpClass(int aInt)
      : mInt(aInt)
    {
      std::cout << "Instance creation :mInt :" << mInt << std::endl;
    };
    int DValue()
    {
      return mInt;
    }

    bool operator==(const TmpClass& other) const noexcept
    {
      return mInt == other.mInt;
    }
  };

  std::vector<TmpClass> TmpClassVec{
    TmpClass(1), TmpClass(2), TmpClass(2), TmpClass(3)
  };

  TmpClassVec.erase(
    std::remove(std::begin(TmpClassVec), std::end(TmpClassVec), TmpClass(2)),
    std::end(TmpClassVec));

  for (auto& value : TmpClassVec)
  {
    std::cout << "Value:" << value.DValue() << std::endl;
  }
  PlaygroundLib::MyUniquePtr<TmpClass> ptr1 =
    PlaygroundLib::make_unique<TmpClass>(10);

  PlaygroundLib::MySharedPtr<TmpClass> ptr2 =
    PlaygroundLib::make_shared<TmpClass>(20);


  LOG_END_FUNCTION();
}

class PtrDemo : public DemoBase
{
public:
  PtrDemo()
  {
    mName = "PtrDemo";
    mNotes = {};
  };
  ~PtrDemo() = default;

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
    MyUniquePtrDemo();
  }

  void ShowDemo(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  };
};