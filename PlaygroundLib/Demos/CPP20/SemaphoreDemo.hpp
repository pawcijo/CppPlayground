#pragma once

#include "Common/DemoBase.hpp"
#include <semaphore>
#include <thread>

class SemaphoreDemo : public DemoBase
{
public:
  SemaphoreDemo()
  {
    mName = "SemaphoreDemo";
    mNotes = { { "<semaphore>",
                 "header provides synchronization primitives for "
                 "controlling access to shared resources." },
               { "std::counting_semaphore",
                 "A counting semaphore that allows a certain "
                 "number of threads to access a resource." },
               { "std::binary_semaphore",
                 "A binary semaphore that allows only one thread "
                 "to access a resource at a time." },
               { "std::timed_wait",
                 "Allows waiting for a semaphore with a timeout." },
               { "std::try_acquire",
                 "Attempts to acquire a semaphore without blocking." } };
  }

  void ShowExample(
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

    CountingSemaphoreExample();
    BinarySemaphoreExample();
    TimedWaitExample();
    TryAcquireExample();

    LOG_END_FUNCTION_CLASS();
  }

  void ShowDemo(
    std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  }

private:
  void CountingSemaphoreExample()
  {
    LOG_START_FUNCTION();
    // Example of using a counting semaphore

    constexpr int MAX_RESOURCES = 3;
    std::counting_semaphore<MAX_RESOURCES> sem(
      MAX_RESOURCES); // counting semaphore

    std::function<void(int)> worker; // named function-style object

    worker = [&sem](int id)
    {
      std::cout << "Worker " << id << " czeka na dostęp...\n";
      sem.acquire();
      std::cout << "Worker " << id << " uzyskał dostęp!\n";

      std::this_thread::sleep_for(std::chrono::seconds(2));

      std::cout << "Worker " << id << " kończy i zwalnia zasób.\n";
      sem.release();
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i)
    {
      threads.emplace_back(worker, i);
    }

    for (auto& t : threads)
    {
      t.join();
    }

    std::cout << "Wszystkie wątki zakończyły pracę.\n";

    LOG_END_FUNCTION();
  }

  void BinarySemaphoreExample()
  {
    LOG_START_FUNCTION();

    std::binary_semaphore bin_sem(1); // binary semaphore initialized to 1

    auto worker = [&bin_sem](const std::string& name)
    {
      std::cout << name << " is waiting for access...\n";
      bin_sem.acquire(); // wait for the semaphore
      std::cout << name << " acquired access!\n";

      std::this_thread::sleep_for(std::chrono::seconds(2)); // simulate work

      std::cout << name << " is done and releasing the resource.\n";
      bin_sem.release(); // release the semaphore
    };

    std::thread t1(worker, "Worker A");
    std::thread t2(worker, "Worker B");

    t1.join();
    t2.join();

    std::cout << "Both workers have finished.\n";

    LOG_END_FUNCTION();
  }

  void TimedWaitExample()
  {
    LOG_START_FUNCTION();
    // Example of timed wait on a semaphore
    std::counting_semaphore<5> semaphore(3); // Allow up to 3 threads

    if (semaphore.try_acquire_for(std::chrono::seconds(1)))
    {
      std::cout << "Acquired semaphore within timeout." << std::endl;
      semaphore.release();
    }
    else
    {
      std::cout << "Failed to acquire semaphore within timeout." << std::endl;
    }

    LOG_END_FUNCTION();
  }
  void TryAcquireExample()
  {
    LOG_START_FUNCTION();
    // Example of trying to acquire a semaphore
    std::counting_semaphore<5> semaphore(3); // Allow up to 3 threads

    if (semaphore.try_acquire())
    {
      std::cout << "Successfully acquired semaphore." << std::endl;
      semaphore.release();
    }
    else
    {
      std::cout << "Failed to acquire semaphore." << std::endl;
    }
  }
};