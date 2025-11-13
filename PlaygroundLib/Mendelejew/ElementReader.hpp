#pragma once
#include "Element.hpp"
#include <filesystem>
#include <vector>
namespace PlaygroundLib
{
class ElementReader
{
public:
  ElementReader() = default;
  ~ElementReader() = default;

   std::vector<Element> ReadElementsFromFile(
    const std::filesystem::path& filePath);
};
}
