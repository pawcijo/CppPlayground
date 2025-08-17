#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "temporary.hpp"

class VulkanWindow
{
public:
  VulkanWindow();
  ~VulkanWindow();

  static void framebufferResizeCallback(GLFWwindow* window,
                                        int width,
                                        int height);

  void initWindow();
  GLFWwindow* window;
  bool framebufferResized = false;
};