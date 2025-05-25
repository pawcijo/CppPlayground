#pragma once

#include <memory_resource>  // Include for memory_resource
#include <thread>

#include "Common/DemoBase.hpp"

class MemoryResourceDemo : public DemoBase
{
   public:
    MemoryResourceDemo()
    {
        mName = "MemoryResourceDemo";
        mNotes = {
            {"memory_resource",
             R"(std::pmr::memory_resource is part of the Polymorphic Memory Resources (PMR) introduced in C++17. 
It provides an abstraction for memory allocation,
allowing you to use different memory management strategies without changing 
code that depends on them.)"}};
    }

    void PrintNotes() override
    {
        PrintName();
        for (const auto &pair : mNotes)
        {
            std::cout << BOLD_TEXT_START << pair.first << TEXT_FORMAT_CLEAR << std::endl
                      << pair.second << std::endl
                      << std::endl;
        }
    }

    void ShowExample(void(*printNotesCallback)(NoteFormat& notes) = nullptr) override
    {
          if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);;
        }
        BasicExample();
        ShowMemoryResource();
        ManuallyResetMemoryResource();

        DemoNewDeleteResource();           // Default heap allocator
        DemoMonotonicBufferResource();     // Stack-like fast allocation
        DemoUnsynchronizedPoolResource();  // Fast pool-based allocation (not thread-safe)
        DemoSynchronizedPoolResource();    // Thread-safe pool allocator
        DemoMultithreading();              // Demonstr
    }

    void ShowDemo() override
    {
        ShowExample();
    }

    void BasicExample()
    {
        LOG_FUNCTION_NAME(YELLOW);

        // Create a new memory resource
        std::pmr::monotonic_buffer_resource pool;
        std::pmr::memory_resource *mr = &pool;

        // Allocate memory
        void *p = mr->allocate(100);

        // Use the memory
        std::cout << "Memory allocated at: " << p << std::endl;

        // Deallocate memory
        mr->deallocate(p, 100);
    }

    void ShowMemoryResource()
    {
        LOG_FUNCTION_NAME(YELLOW);

        // Create a new memory resource
        std::pmr::monotonic_buffer_resource pool;
        std::pmr::memory_resource *mr = &pool;

        // Allocate memory
        void *p = mr->allocate(100);

        // Use the memory
        std::cout << "Memory allocated at: " << p << std::endl;

        // Deallocate memory
        mr->deallocate(p, 100);
    }

    void ManuallyResetMemoryResource()
    {
        LOG_FUNCTION_NAME(YELLOW);

        // Create a new memory resource
        std::pmr::monotonic_buffer_resource pool;
        std::pmr::memory_resource *mr = &pool;

        // Allocate memory
        void *p = mr->allocate(100);

        // Use the memory
        std::cout << "Memory allocated at: " << p << std::endl;

        // Deallocate memory
        mr->deallocate(p, 100);

        // Reset the memory resource
        pool.release();
    }

    // Helper function to demonstrate memory resource
    void DemoMemoryResource(std::pmr::memory_resource *resource)
    {
        std::pmr::vector<int> pmrVector(resource);  // Allocate vector using the given resource
        pmrVector.push_back(42);
        pmrVector.push_back(99);

        std::cout << "PMR Vector Elements: ";
        for (int value : pmrVector) std::cout << value << " ";
        std::cout << "\n";
    }

    // 1️⃣ **Using `std::pmr::new_delete_resource` (Default Heap Allocator)**
    void DemoNewDeleteResource()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::cout << "🔹 Using NewDeleteResource (Default Heap Allocator)\n";
        std::pmr::memory_resource *resource = std::pmr::new_delete_resource();
        DemoMemoryResource(resource);
    }

    // 2️⃣ **Using `std::pmr::monotonic_buffer_resource` (Fast Stack-Like Allocator)**
    void DemoMonotonicBufferResource()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::cout << "🔹 Using MonotonicBufferResource (Stack-Like Allocator)\n";

        char buffer[1024];  // Pre-allocated buffer for efficiency
        std::pmr::monotonic_buffer_resource resource(buffer, sizeof(buffer));

        DemoMemoryResource(&resource);
    }

    // 3️⃣ **Using `std::pmr::unsynchronized_pool_resource` (Optimized for Small Allocations)**
    void DemoUnsynchronizedPoolResource()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::cout << "🔹 Using UnsynchronizedPoolResource (Optimized for Small Objects)\n";

        std::pmr::unsynchronized_pool_resource resource;
        DemoMemoryResource(&resource);
    }

    // 4️⃣ **Using `std::pmr::synchronized_pool_resource` (Thread-Safe Version)**
    void DemoSynchronizedPoolResource()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::cout << "🔹 Using SynchronizedPoolResource (Thread-Safe Version)\n";

        std::pmr::synchronized_pool_resource resource;
        DemoMemoryResource(&resource);
    }

    // 5️⃣ **Threaded Test for SynchronizedPoolResource**
    void DemoMultithreading()
    {
        LOG_FUNCTION_NAME(YELLOW);
        std::pmr::synchronized_pool_resource sharedResource;

        auto ThreadFunction = [&](int threadId)
        {
            std::pmr::vector<int> pmrVector(&sharedResource);
            pmrVector.push_back(threadId);
            pmrVector.push_back(threadId * 2);
            std::cout << "Thread " << threadId << " added: " << pmrVector[0] << ", " << pmrVector[1]
                      << "\n";
        };

        std::thread thread1(ThreadFunction, 1);
        std::thread thread2(ThreadFunction, 2);

        thread1.join();
        thread2.join();
    }
};