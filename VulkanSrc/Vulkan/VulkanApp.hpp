#pragma once

#include <glm/glm.hpp>

class VulkanPipeLine;
class VulkanApp
{
private:
  VulkanPipeLine& mGraphicPipeline;

  // Clean up
  void CleanUp();

public:
  VulkanApp(VulkanPipeLine& window);
  ~VulkanApp();
  void Run();

  friend class VulkanPipeLine;
};
