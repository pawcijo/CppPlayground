#include "PersonalComputer.hpp"

#include "PcBuilder.hpp"
#include <string>

PcBuilder& PcBuilder::setName(std::string& name)
{
  mPc.setName(name);
  return *this;
}
PcBuilder& PcBuilder::setCpu(std::string& cpu)
{
  mPc.setCpu(cpu);
  return *this;
}
PcBuilder& PcBuilder::setMotherboard(std::string& motherboard)
{
  mPc.setMotherboard(motherboard);
  return *this;
}
PcBuilder& PcBuilder::setRam(std::vector<std::string>& ramList)
{
  mPc.setRam(ramList);
  return *this;
}
PcBuilder& PcBuilder::setGpu(std::string& gpu)
{
  mPc.setGpu(gpu);
  return *this;
}
PcBuilder& PcBuilder::setStorage(std::vector<std::string>& storageList)
{
  mPc.setStorage(storageList);
  return *this;
}
PcBuilder& PcBuilder::setPowerSupply(std::string& powerSupply)
{
  mPc.setPowerSupply(powerSupply);
  return *this;
}
PcBuilder& PcBuilder::setCase(std::string& pcCase)
{
  mPc.setCase(pcCase);
  return *this;
}
PcBuilder& PcBuilder::setCooling(std::string& cooling)
{
  mPc.setCooling(cooling);
  return *this;
}
