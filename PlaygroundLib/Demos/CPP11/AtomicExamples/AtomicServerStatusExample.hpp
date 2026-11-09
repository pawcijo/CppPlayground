
#pragma once

#include <atomic>
#include <print>
#include <thread>

#include "Common/Common.hpp"

namespace PlaygroundLib
{
namespace AtomicExamples
{

[[maybe_unused]] static void ServerStatusWithoutAtomic()
{
  LOG_START_FUNCTION();

  constexpr int kIterations = 1'000'000;

  bool ServerRunning = true;
  // volatile only stops the compiler from folding the loop into one add;
  // it does NOT make the access thread-safe.
  volatile int RequestsHandled = 0;

  std::jthread Server(
    [&]()
    {
      std::println("Server: started");

      for (int i = 0; i < kIterations; ++i)
      {
        RequestsHandled = RequestsHandled + 1; // read-modify-write, not atomic
      }

      ServerRunning = false; // also racy, read by Monitor below
      std::println("Server: stopped");
    });

  std::jthread Monitor(
    [&]()
    {
      for (int i = 0; i < kIterations; ++i)
      {
        RequestsHandled = RequestsHandled + 1; // races with Server
      }

      std::println("Monitor: finished counting, ServerRunning = {}",
                   ServerRunning);
    });

  Server.join();
  Monitor.join();

  std::println("Expected: {}  Actual: {}  Lost updates: {}",
               2 * kIterations,
               static_cast<int>(RequestsHandled),
               2 * kIterations - static_cast<int>(RequestsHandled));

  LOG_END_FUNCTION();
}

[[maybe_unused]] static void ServerStatusWithAtomic()
{
  LOG_START_FUNCTION();

  constexpr int kIterations = 1'000'000;

  std::atomic<bool> ServerRunning{ true };
  std::atomic<int> RequestsHandled{ 0 };

  std::jthread Server(
    [&]()
    {
      std::println("Server: started");

      for (int i = 0; i < kIterations; ++i)
      {
        RequestsHandled.fetch_add(
          1, std::memory_order_relaxed); // atomic read-modify-write
      }

      ServerRunning.store(false);
      std::println("Server: stopped");
    });

  std::jthread Monitor(
    [&]()
    {
      for (int i = 0; i < kIterations; ++i)
      {
        RequestsHandled.fetch_add(1, std::memory_order_relaxed);
      }

      std::println("Monitor: finished counting, ServerRunning = {}",
                   ServerRunning.load());
    });

  Server.join();
  Monitor.join();

  const int Actual = RequestsHandled.load();
  std::println("Expected: {}  Actual: {}  Lost updates: {}",
               2 * kIterations,
               Actual,
               2 * kIterations - Actual);

  LOG_END_FUNCTION();
}

}
}