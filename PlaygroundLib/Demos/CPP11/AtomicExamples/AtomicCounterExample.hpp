#pragma once

#include <atomic>
#include <print>
#include <thread>

#include "Common/Common.hpp"

namespace PlaygroundLib
{
namespace AtomicExamples
{

[[maybe_unused]] static void CounterWithoutAtomic()
{
  LOG_START_FUNCTION();

  int Counter = 0;

  constexpr int ThreadCount = 4;
  constexpr int Iterations = 1'000'000;

  auto Worker = [&]()
  {
    for (int i = 0; i < Iterations; ++i)
    {
      ++Counter;
    }
  };

  {
    std::array<std::jthread, ThreadCount> Threads;

    for (auto& Thread : Threads)
    {
      Thread = std::jthread(Worker);
    }
  }

  std::println("Expected: {}", ThreadCount * Iterations);
  std::println("Actual:   {}", Counter);

  LOG_END_FUNCTION();
}

[[maybe_unused]] static void CounterWithAtomic()
{
  LOG_START_FUNCTION();
  std::atomic<int> Counter{ 0 };

  constexpr int ThreadCount = 4;
  constexpr int Iterations = 1'000'000;

  auto Worker = [&]()
  {
    for (int i = 0; i < Iterations; ++i)
    {
      Counter.fetch_add(1, std::memory_order_relaxed);
    }
  };

  {
    std::array<std::jthread, ThreadCount> Threads;

    for (auto& Thread : Threads)
    {
      Thread = std::jthread(Worker);
    }
  }

  std::println("Expected: {}", ThreadCount * Iterations);
  std::println("Actual:   {}", Counter.load());

  LOG_END_FUNCTION();
}
}
}