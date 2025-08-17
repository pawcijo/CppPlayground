#include "VulkanFunApp.hpp"

#include <cstring>
#include <cstdlib>

#include "VulkanValidation.hpp"
#include "temporary.hpp"

#include <vector>
#include <stdexcept>
#include <chrono>

VulkanFunApp::VulkanFunApp()
  : window(new VulkanWindow())
  , pipeline(nullptr)
{
}

void VulkanFunApp::run()
{
  pipeline = new VulkanPipeline(*window->window);
  initVulkan();
  mainLoop();
  cleanup();
}

void VulkanFunApp::initVulkan()
{
  pipeline->initVulkan();
}

void VulkanFunApp::mainLoop()
{
  while (!glfwWindowShouldClose(window->window))
  {
    glfwPollEvents();
    drawFrame();
  }
  vkDeviceWaitIdle(pipeline->device);
}

void VulkanFunApp::cleanup()
{
  pipeline->cleanupSwapChain();

  pipeline->DestroyPipeline();
}

void VulkanFunApp::recreateSwapChain()
{
  int width = 0, height = 0;
  glfwGetFramebufferSize(window->window, &width, &height);
  while (width == 0 || height == 0)
  {
    glfwGetFramebufferSize(window->window, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(pipeline->device);

  pipeline->cleanupSwapChain();
  pipeline->createSwapChain();
  pipeline->createImageViews();
  pipeline->createFramebuffers();
}

void VulkanFunApp::populateDebugMessengerCreateInfo(
  VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = VulkanValidation::debugCallback;
}

void VulkanFunApp::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = pipeline->renderPass;
  renderPassInfo.framebuffer = pipeline->swapChainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = { 0, 0 };
  renderPassInfo.renderArea.extent = pipeline->swapChainExtent;

  VkClearValue clearColor = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)pipeline->swapChainExtent.width;
  viewport.height = (float)pipeline->swapChainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = { 0, 0 };
  scissor.extent = pipeline->swapChainExtent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  VkBuffer vertexBuffers[] = { pipeline->vertexBuffer };
  VkDeviceSize offsets[] = { 0 };
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

  vkCmdBindIndexBuffer(commandBuffer, pipeline->indexBuffer, 0, VK_INDEX_TYPE_UINT16);

  vkCmdBindDescriptorSets(commandBuffer,
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline->pipelineLayout,
                          0,
                          1,
                          &pipeline->descriptorSets[currentFrame],
                          0,
                          nullptr);

  vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

  vkCmdEndRenderPass(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void VulkanFunApp::updateUniformBuffer(uint32_t currentImage)
{
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                 currentTime - startTime)
                 .count();

  UniformBufferObject ubo{};
  ubo.model = glm::rotate(
    glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(glm::radians(45.0f),
                              pipeline->swapChainExtent.width /
                                (float)pipeline->swapChainExtent.height,
                              0.1f,
                              10.0f);
  ubo.proj[1][1] *= -1;

  memcpy(pipeline->uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void VulkanFunApp::drawFrame()
{
  vkWaitForFences(pipeline->device,
                  1,
                  &pipeline->inFlightFences[currentFrame],
                  VK_TRUE,
                  UINT64_MAX);

  uint32_t imageIndex;
  VkResult result =
    vkAcquireNextImageKHR(pipeline->device,
                          pipeline->swapChain,
                          UINT64_MAX,
                          pipeline->imageAvailableSemaphores[currentFrame],
                          VK_NULL_HANDLE,
                          &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    recreateSwapChain();
    return;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  updateUniformBuffer(currentFrame);

  vkResetFences(pipeline->device, 1, &pipeline->inFlightFences[currentFrame]);

  vkResetCommandBuffer(pipeline->commandBuffers[currentFrame], 0);
  recordCommandBuffer(pipeline->commandBuffers[currentFrame], imageIndex);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {
    pipeline->imageAvailableSemaphores[currentFrame]
  };
  VkPipelineStageFlags waitStages[] = {
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
  };
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &pipeline->commandBuffers[currentFrame];

  VkSemaphore signalSemaphores[] = {
    pipeline->renderFinishedSemaphores[currentFrame]
  };
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(pipeline->graphicsQueue,
                    1,
                    &submitInfo,
                    pipeline->inFlightFences[currentFrame]) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = { pipeline->swapChain };
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;

  presentInfo.pImageIndices = &imageIndex;

  result = vkQueuePresentKHR(pipeline->presentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window->framebufferResized)
  {
    window->framebufferResized = false;
    recreateSwapChain();
  }
  else if (result != VK_SUCCESS)
  {
    throw std::runtime_error("failed to present swap chain image!");
  }

  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}