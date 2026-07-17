#pragma once

#include <vulkan/vulkan.h>

#include "nib/result.h"

namespace nib
{

enum class Renderer_Error
{
  Failed_To_Create_VkInstance,
};

struct Renderer
{
  static Result<Renderer, Renderer_Error> new_();
  ~Renderer();

  Renderer(const Renderer& other) = delete;
  Renderer& operator=(const Renderer& other) = delete;

  Renderer(Renderer&& other) : instance(other.instance) { other.instance = VK_NULL_HANDLE; }

 private:
  Renderer(VkInstance instance) : instance(instance) {}

  VkInstance instance;
};

}  // namespace nib