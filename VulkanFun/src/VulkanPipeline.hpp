#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <vulkan/vulkan_core.h>

class VulkanFunApp;
class VulkanPipeline
{

public:
  VulkanPipeline(GLFWwindow& window)
    : window(window)
  {
  }
  void initVulkan();

  void cleanup()
  {
  }

  void cleanupSwapChain();

private:
  GLFWwindow& window;
  VkSurfaceKHR surface;
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;

  VkQueue graphicsQueue;
  VkQueue presentQueue;

  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;

  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;

  std::vector<VkImageView> swapChainImageViews;
  std::vector<VkFramebuffer> swapChainFramebuffers;

  VkRenderPass renderPass;

  VkDescriptorSetLayout descriptorSetLayout;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;

  VkCommandPool commandPool;

  VkImage textureImage;
  VkDeviceMemory textureImageMemory;
  VkImageView textureImageView;
  VkSampler textureSampler;

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;

  std::vector<VkBuffer> uniformBuffers;
  std::vector<VkDeviceMemory> uniformBuffersMemory;
  std::vector<void*> uniformBuffersMapped;

  VkDescriptorPool descriptorPool;
  std::vector<VkDescriptorSet> descriptorSets;

  std::vector<VkCommandBuffer> commandBuffers;

  std::vector<VkSemaphore> imageAvailableSemaphores;
  std::vector<VkSemaphore> renderFinishedSemaphores;
  std::vector<VkFence> inFlightFences;

  void createInstance();

  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain();
  void createImageViews();
  void createRenderPass();
  void createDescriptorSetLayout();
  void createGraphicsPipeline();
  void createFramebuffers();
  void createCommandPool();
  void createTextureImage();
  void createTextureImageView();
  void createTextureSampler();
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffers();
  void createDescriptorPool();
  void createDescriptorSets();
  void createCommandBuffers();
  void createSyncObjects();

  VkImageView createImageView(VkImage image, VkFormat format);

  friend class VulkanFunApp;
};