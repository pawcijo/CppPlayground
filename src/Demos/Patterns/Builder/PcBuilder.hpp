#pragma once

#include <memory>
#include <string>
#include <vector>
#include <memory>
#include "PersonalComputer.hpp"

class PcBuilder
{
public:
  PcBuilder() = default;
  ~PcBuilder() = default;

  PcBuilder& setName(std::string& name);
  PcBuilder& setCpu(std::string& cpu);
  PcBuilder& setMotherboard(std::string& motherboard);
  PcBuilder& setRam(std::vector<std::string>& ramList);
  PcBuilder& setGpu(std::string& gpu);
  PcBuilder& setStorage(std::vector<std::string>& storageList);
  PcBuilder& setPowerSupply(std::string& powerSupply);
  PcBuilder& setCase(std::string& pcCase);
  PcBuilder& setCooling(std::string& cooling);

  std::unique_ptr<PersonalComputer> build()
  {
    return std::make_unique<PersonalComputer>(std::move(mPc));
  }

private:
  PersonalComputer mPc;
};