#include "VulkanWindow.hpp"

VulkanWindow::VulkanWindow()
  : window(nullptr)
  , framebufferResized(false)
{
  initWindow();
}

VulkanWindow::~VulkanWindow()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

void VulkanWindow::framebufferResizeCallback(GLFWwindow* window,
                                             int width,
                                             int height)
{
  auto app = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
  app->framebufferResized = true;
}

void VulkanWindow::initWindow()
{
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan fun app", nullptr, nullptr);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}