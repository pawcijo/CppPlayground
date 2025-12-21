#pragma once
#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include <chrono>
#include <future>
#include <string>
#include <thread>

class FutureObjDemo : public DemoBase
{

  class Example
  {
    std::string value;

  public:
    Example(const std::string& val)
      : value(val)
    {
    }

    Example(const Example& other)
      : value(other.value)
    {
    }

    Example(const Example&& other)
      : value(std::move(other.value))
    {
    }

    const std::string& GetValue() const
    {
      return value;
    }

    void SetValue(const std::string& val)
    {
      value = val;
    }

    Example& operator=(const Example&& other)
    {
      value = std::move(other.value);
      return *this;
    }
  };

  Example example{ "Initial Value" };

public:
  FutureObjDemo()
  {
    mName = "FutureObjDemo";
    mNotes = {
      { "std::future:",
        R"(The std::future is a C++11 feature that provides a way to access the result of an asynchronous operation. 
It is part of the C++ Standard Library's concurrency support and is used in conjunction with std::promise and std::async.)" }
    };
  };
  ~FutureObjDemo() override = default;

  void ShowExample(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    if (nullptr == printNotesCallback)
    {
      PrintNotes();
    }
    else
    {
      printNotesCallback(mNotes);
    }

    // create a promise object
    std::future<Example> fut = do_work(std::chrono::milliseconds(2000));

    std::cout << "Waiting for the result...\n";
    // wait for the result
    fut.wait();
    Example result = fut.get();
    std::cout << "Result received: " << result.GetValue() << "\n";
  }

  // create a thread and set the promise value after some time to simulate
  // random time
  std::future<Example> do_work(std::chrono::milliseconds runningTime)
  {

    std::promise<Example> promise;
    std::future<Example> future = promise.get_future();

    std::thread(
      [runningTime, promise = std::move(promise)]() mutable
      {
        std::this_thread::sleep_for(runningTime);
        promise.set_value(Example("Updated Value after work"));
      })
      .detach();

    return future; // ✅
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }
};