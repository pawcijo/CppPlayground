#pragma once

#include "VulkanWindow.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanValidation.hpp"
#include "VulkanUtils.hpp"
#include "temporary.hpp"

#include <vector>
#include <stdexcept>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class VulkanFunApp
{
public:
  VulkanFunApp();
  void run();

private:
  VulkanWindow* window;
  VulkanPipeline* pipeline;
  uint32_t currentFrame = 0;

  void initVulkan();
  void mainLoop();
  void cleanup();
  void recreateSwapChain();
  void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
  void updateUniformBuffer(uint32_t currentImage);
  void drawFrame();
};