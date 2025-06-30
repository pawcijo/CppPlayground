#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"

#include <coroutine>

#include <cstdint>

namespace
{
class Generator
{
public:
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type
  {
    unsigned long long current_value; // unsigned long long to hold the value 
                                      // uint64_t could also work
    std::suspend_always yield_value(unsigned long long value)
    {
      current_value = value;
      return {};
    }

    std::suspend_always initial_suspend()
    {
      return {};
    }
    std::suspend_always final_suspend() noexcept
    {
      return {};
    }

    Generator get_return_object()
    {
      return Generator{ handle_type::from_promise(*this) };
    }

    void return_void()
    {
    }
    void unhandled_exception()
    {
      std::exit(1);
    }
  };

  Generator(handle_type h)
    : coro(h)
  {
  }
  ~Generator()
  {
    if (coro)
      coro.destroy();
  }

  // Iterator-style access
  bool next()
  {
    LOG_MSG_FUNCTION("Generator next called");
    if (!coro.done())
    {
      coro.resume();
    }
    return !coro.done();
  }

  unsigned long long value() const
  {
    return coro.promise().current_value;
  }

private:
  handle_type coro;
};

}

class CoroutineDemo : public DemoBase
{
public:
  CoroutineDemo()
  {
    mName = "CoroutineDemo";
    mNotes = {
      { "<coroutine>", "header provides support for coroutines." },
      { "co_await",
        "A keyword that suspends the execution of a coroutine until the "
        "awaited operation is complete." },
      { "co_yield",
        "A keyword that produces a value from a coroutine and "
        "suspends its execution." },
      { "co_return",
        "A keyword that returns a value from a coroutine and "
        "completes its execution." },
      { "std::coroutine_handle",
        "A type that represents a handle to a coroutine." },
      { "std::suspend_always",
        "A type that always suspends the execution of a coroutine." },
      { "std::suspend_never",
        "A type that never suspends the execution of a coroutine." },
      { "Why to use coroutines?",
        "Coroutines allow writing asynchronous code in a more "
        "synchronous style, making it easier to read and maintain."
        "You could generate an infinite calls without using infinite memory." },
    };
  }

  ~CoroutineDemo() = default;

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    LOG_START_FUNCTION_CLASS();
    if (nullptr == printNotesCallback)
    {
      PrintNotes();
    }
    else
    {
      printNotesCallback(mNotes);
    }

    // Add coroutine examples here
    // For example, you can create a simple generator coroutine
    CoroutineExample();
    FibonacciExample();

    LOG_END_FUNCTION_CLASS();
  }

  void ShowExample(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowDemo(printNotesCallback);
  }

  Generator counter(int max)
  {
    for (int i = 1; i <= max; ++i)
    {
      co_yield i;
    }
  }

  void CoroutineExample()
  {
    LOG_START_FUNCTION();
    // Example of a simple coroutine
    // This is just a placeholder for your coroutine logic

    std::cout << "Coroutine example executed." << std::endl;

    Generator gen = counter(5);

    while (gen.next())
    {
      std::cout << "Got value: " << gen.value() << '\n';
    }

    LOG_END_FUNCTION();
  };

  void FibonacciExample()
  {
    LOG_START_FUNCTION();
    // Example of a Fibonacci coroutine
    Generator fibonacci = [](int n) -> Generator
    {
      unsigned long long a = 0, b = 1;
      for (int i = 0; i < n; ++i)
      {
        co_yield a;
        unsigned long long next = a + b;
        a = b;
        b = next;
      }
    }(50);

    int counter = 0;
    while (fibonacci.next())
    {

      std::cout << "Fibonacci value for " << counter << ":" << fibonacci.value()
                << '\n';
      counter++;
    }

    LOG_END_FUNCTION();
  }
};