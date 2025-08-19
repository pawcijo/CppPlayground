#pragma once

#include <cstdint>
#include "Vertex.hpp"
#include "glm/ext/scalar_constants.hpp"
#include <glm/glm.hpp>
#include <optional>
#include <vector>


const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

struct UniformBufferObject
{
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

constexpr float PI = glm::pi<float>();
constexpr std::vector<Vertex> makeSphereVertices(uint32_t sectorCount, uint32_t stackCount, float radius = 1.0f) {
    std::vector<Vertex> vertices;
    vertices.reserve((stackCount + 1) * (sectorCount + 1));

    for (uint32_t i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2.0f - (float)i * (PI / stackCount); // from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (uint32_t j = 0; j <= sectorCount; ++j) {
            float sectorAngle = (float)j * (2.0f * PI / sectorCount); // 0..2pi

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            glm::vec3 pos = {x, y, z};

            // Color: encode normalized position (like normal-based coloring)
            glm::vec3 color = glm::normalize(pos) * 0.5f + 0.5f;

            // Texcoords
            float u = (float)j / sectorCount;
            float v = (float)i / stackCount;

            vertices.push_back({pos, color, {u, v}});
        }
    }

    return vertices;
}

constexpr std::vector<uint16_t> makeSphereIndices(uint32_t sectorCount, uint32_t stackCount) {
    std::vector<uint16_t> indices;
    indices.reserve(stackCount * sectorCount * 6);

    for (uint32_t i = 0; i < stackCount; ++i) {
        uint32_t k1 = i * (sectorCount + 1);     // beginning of current stack
        uint32_t k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back((uint16_t)k1);
                indices.push_back((uint16_t)k2);
                indices.push_back((uint16_t)(k1 + 1));
            }

            if (i != (stackCount - 1)) {
                indices.push_back((uint16_t)(k1 + 1));
                indices.push_back((uint16_t)k2);
                indices.push_back((uint16_t)(k2 + 1));
            }
        }
    }
    return indices;
}

const std::vector<Vertex> vertices = makeSphereVertices(16, 32);
const std::vector<uint16_t> indices = makeSphereIndices(16, 32);

/*
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};
*/

struct QueueFamilyIndices
{
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete()
  {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};