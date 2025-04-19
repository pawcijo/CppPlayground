#pragma once

#include <iostream>
#include <thread>
#include <mutex>

// Scott Meyers Singleton
// https://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html
class Singleton {
public:
    // Deleted methods to prevent copying or assignment
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& getInstance() {
        // Guaranteed to be thread-safe in C++11 and later
        static Singleton instance;
        return instance;
    }

    void doSomething() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "Doing something in thread " << std::this_thread::get_id() << std::endl;
    }

private:
    Singleton() {
        std::cout << "Singleton constructor called\n";
    }

    ~Singleton() {
        std::cout << "Singleton destructor called\n";
    }

    std::mutex mutex_;  // For synchronizing internal operations
};
