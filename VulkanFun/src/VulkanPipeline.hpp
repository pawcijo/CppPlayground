#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <vulkan/vulkan_core.h>
#include "VulkanValidation.hpp"

#include "temporary.hpp"
#include "VulkanUtils.hpp"

class VulkanFunApp;
class VulkanPipeline
{

public:
  VulkanPipeline(GLFWwindow& window)
    : window(window)
  {
  }
  void initVulkan();

  void DestroyPipeline()
  {
      vkDestroyPipeline( device,  graphicsPipeline, nullptr);
  vkDestroyPipelineLayout( device,  pipelineLayout, nullptr);
  vkDestroyRenderPass( device,  renderPass, nullptr);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroyBuffer( device,  uniformBuffers[i], nullptr);
    vkFreeMemory( device,  uniformBuffersMemory[i], nullptr);
  }

  vkDestroyDescriptorPool( device,  descriptorPool, nullptr);
  vkDestroySampler( device,  textureSampler, nullptr);
  vkDestroyImageView( device,  textureImageView, nullptr);

  vkDestroyImage( device,  textureImage, nullptr);
  vkFreeMemory( device,  textureImageMemory, nullptr);

  vkDestroyDescriptorSetLayout( device,  descriptorSetLayout, nullptr);

  vkDestroyBuffer( device,  indexBuffer, nullptr);
  vkFreeMemory( device,  indexBufferMemory, nullptr);

  vkDestroyBuffer( device,  vertexBuffer, nullptr);
  vkFreeMemory( device,  vertexBufferMemory, nullptr);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
  {
    vkDestroySemaphore( device,  renderFinishedSemaphores[i], nullptr);
    vkDestroySemaphore( device,  imageAvailableSemaphores[i], nullptr);
    vkDestroyFence( device,  inFlightFences[i], nullptr);
  }

  vkDestroyCommandPool( device,  commandPool, nullptr);
  vkDestroyDevice( device, nullptr);

  if (enableValidationLayers)
  {
    VulkanValidation::DestroyDebugUtilsMessengerEXT(
       instance,  debugMessenger, nullptr);
  }

  vkDestroySurfaceKHR( instance,  surface, nullptr);
  vkDestroyInstance( instance, nullptr);

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