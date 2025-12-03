#pragma once

#include <utility>

namespace PlaygroundLib
{
template<typename T>
class MySharedPtr
{

  T* mPtr;
  long* ref_counter;

  void cleanup()
  {

    (void)*ref_counter--;
    if (ref_counter == 0)
    {
      if (nullptr != mPtr)
      {
        delete mPtr;
      }
      delete ref_counter;
    }
  }

public:
  // default constructor
  explicit MySharedPtr() noexcept
    : mPtr(nullptr)
    , ref_counter(new long(0))
  {
  }

  // Constructr
  //
  explicit MySharedPtr(T* ptr) noexcept
    : mPtr(ptr)
    , ref_counter(new long(0))
  {
  }

  // Copy Constructor
  MySharedPtr(const MySharedPtr& other) noexcept
  {
    mPtr = other.mPtr;
    ref_counter = other.ref_counter;

    if (nullptr != ref_counter)
    {
      (*ref_counter)++;
    }
  }

  // Copy assign operator
  MySharedPtr& operator=(const MySharedPtr& other) noexcept
  {

    cleanup();

    mPtr = other.mPtr;
    ref_counter = other.ref_counter;

    if (nullptr != ref_counter)
    {
      (*ref_counter)++;
    }
    return *this;
  }

  // Move Constructor
  MySharedPtr(MySharedPtr&& moved_other) noexcept
    : mPtr(moved_other.mPtr)
    , ref_counter(moved_other.ref_counter)
  {
    moved_other.mPtr = nullptr;
    moved_other.ref_counter = nullptr;
  }

  // Move assign operator
  MySharedPtr& operator=(MySharedPtr&& moved_other) noexcept
  {
    cleanup();

    mPtr = moved_other.mPtr;
    ref_counter = moved_other.ref_counter;

    moved_other.mPtr = nullptr;
    moved_other.ref_counter = nullptr;
  }

  // Destructor
  ~MySharedPtr()
  {
    cleanup();
  }
};

template<typename T, typename... Args>
auto make_shared(Args&&... args) noexcept -> MySharedPtr<T>
{
  return MySharedPtr<T>(new T(std::forward<Args>(args)...));
};

} // namespace PlaygroundLib
