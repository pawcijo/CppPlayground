#pragma once

#include <utility>

namespace PlaygroundLib
{

template<typename T>
class MyUniquePtr
{

  T* mPtr;

public:
  // Constructor
  explicit MyUniquePtr(T* aPtr)
    : mPtr(aPtr)
  {
  }

  ~MyUniquePtr()
  {
    delete mPtr;
  }

  // Move constructor
  MyUniquePtr(MyUniquePtr&& other) noexcept
    : mPtr(other.mPtr)
  {
    other.mPtr = nullptr;
  }

  // Move assign
  MyUniquePtr& operator=(MyUniquePtr&& other) noexcept
  {
    if (nullptr != other.mPtr)
    {
      delete mPtr;
      mPtr = other.mPtr;
      other.mPtr = nullptr;
    }

    return *this;
  }

  // Removed Copy Constructor and removed copy assign operator
  MyUniquePtr(MyUniquePtr& other) = delete;
  MyUniquePtr& operator=(MyUniquePtr& other) = delete;

  T& operator*() const noexcept
  {
    return *mPtr;
  }

  T* operator->() const noexcept
  {
    return mPtr;
  }

  explicit operator bool() const noexcept
  {
    return nullptr != mPtr;
  }

  void reset(T* ptr = nullptr) noexcept
  {
    delete mPtr;
    mPtr = ptr;
  }

  void swap(MyUniquePtr& other)
  {
    std::swap(this, other);

    // or
    T* tmp = mPtr;
    mPtr = other.mPtr;
    other.mPtr = tmp;
  }

  T* get()
  {
    return mPtr;
  }
};

template<typename T, typename... Args>
auto make_unique(Args&&... args) noexcept -> MyUniquePtr<T>
{
    return MyUniquePtr<T>(new T(std::forward<Args>(args)...));
}


}