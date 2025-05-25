#pragma once

#include "Common/Common.hpp"
#include "Common/DemoBase.hpp"
#include "Demos/Patterns/Builder/PcBuilder.hpp"
#include "Demos/Patterns/Builder/PersonalComputer.hpp"
#include <memory>
#include <thread>

class BuilderDemo : public DemoBase
{
public:
  BuilderDemo()
  {
    mName = "BuilderDemo";
    mNotes = {
      { "Builder:",
        R"(
The Builder pattern is a creational design pattern that allows for the step-by-step construction of complex objects.
It separates the construction of a complex object from its representation, allowing the same construction process to create different representations.)" },
      { "",
        R"(The Builder pattern is particularly useful when an object requires a lot of configuration or has many optional parameters.)" },
      { "",
        R"(In C++, the Builder pattern can be implemented using classes and method chaining.)" }
    };
  };
  ~BuilderDemo() = default;

  void ShowExample(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
      if (nullptr == printNotesCallback)
        {
            PrintNotes();
        }
        else
        {
            printNotesCallback(mNotes);
        }

    std::string name = "PC 1";
    std::string name2 = "PC 2";
    std::string cpu = "Intel Core i7";
    std::string motherboard = "ASUS ROG Strix";
    std::vector<std::string> ramList = { "16GB", "32GB" };
    std::string gpu = "NVIDIA GeForce RTX 3080";
    std::vector<std::string> storageList = { "1TB SSD", "2TB HDD" };
    std::string powerSupply = "750W";
    std::string pcCase = "Cooler Master";
    std::string cooling = "Liquid Cooling";

    PcBuilder pcBuilder;

    std::unique_ptr<PersonalComputer> pc = pcBuilder.setName(name)
                                             .setCpu(cpu)
                                             .setMotherboard(motherboard)
                                             .setRam(ramList)
                                             .setGpu(gpu)
                                             .setStorage(storageList)
                                             .setPowerSupply(powerSupply)
                                             .setCase(pcCase)
                                             .setCooling(cooling)
                                             .build();

    std::unique_ptr<PersonalComputer> pc_2 = pcBuilder.setName(name2)
                                               .setCpu(cpu)
                                               .setMotherboard(motherboard)
                                               .setRam(ramList)
                                               .setGpu(gpu)
                                               .setStorage(storageList)
                                               .setPowerSupply(powerSupply)
                                               .setCase(pcCase)
                                               .setCooling(cooling)
                                               .build();

    pc->startPC();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pc_2->startPC();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    pc->stopPC();
    pc_2->stopPC();

    pc->join();
    pc_2->join();
  }

  void ShowDemo(std::function<void(NoteFormat&)> printNotesCallback = nullptr) override
  {
    ShowExample(printNotesCallback);
  };
};