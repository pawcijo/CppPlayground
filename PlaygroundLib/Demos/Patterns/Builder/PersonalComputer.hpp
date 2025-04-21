#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <iostream>

class PcBuilder;

class PersonalComputer
{
private:
  PersonalComputer() = default;

public:
  ~PersonalComputer() = default;

  PersonalComputer(const PersonalComputer& other) = delete;
  PersonalComputer& operator=(const PersonalComputer& other) = delete;

  void showConfiguration()
  {
    std::cout << "CPU: " << mCpu << "\n"
              << "Motherboard: " << mMotherboard << "\n"
              << "GPU: " << mGpu << "\n"
              << "RAM: ";
    for (const auto& ram : mRamList)
    {
      std::cout << ram << " ";
    }
    std::cout << "\nStorage: ";
    for (const auto& storage : mStorageList)
    {
      std::cout << storage << " ";
    }
    std::cout << "\nPower Supply: " << mPowerSupply << "\nCase: " << mCase
              << "\nCooling: " << mCooling << std::endl;
  }

  void startPC()
  {
    std::cout << mName << "::Starting PC..." << std::endl;
    workerThread = std::thread(&PersonalComputer::run, this);
  }

  void stopPC()
  {
    std::cout << mName << "::Sending stop message to PC..." << std::endl;
    {
      std::lock_guard<std::mutex> lock(mtx);
      stopFlag = true;
    }
    {
      std::lock_guard<std::mutex> lock(mtx);
      stopFlag = true;
    }
    cv.notify_one();
  }

  void join()
  {
    if (workerThread.joinable())
      workerThread.join();
  }

  PersonalComputer(const PersonalComputer&& other)
  {
    mName = other.mName;
    mCpu = other.mCpu;
    mMotherboard = other.mMotherboard;
    mGpu = other.mGpu;
    mRamList = other.mRamList;
    mStorageList = other.mStorageList;
    mPowerSupply = other.mPowerSupply;
    mCase = other.mCase;
    mCooling = other.mCooling;
  }

  PersonalComputer& operator=(const PersonalComputer&& other)
  {
    if (this != &other)
    {
      mName = other.mName;
      mCpu = other.mCpu;
      mMotherboard = other.mMotherboard;
      mGpu = other.mGpu;
      mRamList = other.mRamList;
      mStorageList = other.mStorageList;
      mPowerSupply = other.mPowerSupply;
      mCase = other.mCase;
      mCooling = other.mCooling;
    }
    return *this;
  }

private:
  void run()
  {
    std::unique_lock<std::mutex> lock(mtx);
    while (!stopFlag)
    {
      // Wait up to 100ms, or until stopFlag is true
      cv.wait_for(lock,
                  std::chrono::milliseconds(100),
                  [this] { return stopFlag.load(); });

      if (stopFlag)
        break;

      // 🔁 Do your regular work here
      std::cout << mName << "::Working..." << std::endl;
    }
    std::cout << mName << "::Pc turned off(Worker thread exiting.)" << std::endl;
  }

  void setName(const std::string& name)
  {
    mName = name;
  }
  void setCpu(const std::string& cpu)
  {
    mCpu = cpu;
  }
  void setMotherboard(const std::string& motherboard)
  {
    mMotherboard = motherboard;
  }
  void setRam(const std::vector<std::string>& ramList)
  {
    mRamList = ramList;
  }
  void setGpu(const std::string& gpu)
  {
    mGpu = gpu;
  }
  void setStorage(const std::vector<std::string>& storageList)
  {
    mStorageList = storageList;
  }
  void setPowerSupply(const std::string& powerSupply)
  {
    mPowerSupply = powerSupply;
  }
  void setCase(const std::string& pcCase)
  {
    mCase = pcCase;
  }
  void setCooling(const std::string& cooling)
  {
    mCooling = cooling;
  }
  void setRamList(const std::vector<std::string>& ramList)
  {
    mRamList = ramList;
  }
  void setStorageList(const std::vector<std::string>& storageList)
  {
    mStorageList = storageList;
  }

  std::thread workerThread;
  std::mutex mtx;
  std::condition_variable cv;
  std::atomic<bool> stopFlag;

  std::string mName;
  std::string mCpu;
  std::string mMotherboard;
  std::string mGpu; // could be list
  std::vector<std::string> mRamList;
  std::vector<std::string> mStorageList;
  std::string mPowerSupply;
  std::string mCase;
  std::string mCooling;

  friend class PcBuilder;
};