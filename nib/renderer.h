#pragma once

#include <vulkan/vulkan.h>

#include <optional>

#include "nib/result.h"

namespace nib
{

enum class Gfx_Error
{
  Failed_To_Create_VkInstance,
  Failed_To_Get_Adapter,
  Failed_To_Get_Queue_Family,
  Failed_To_Get_Device,
};

struct Device
{
  static Result<Device, Gfx_Error> new_(VkInstance instance, VkSurfaceKHR surface);

  Device(const Device& other) = delete;
  Device& operator=(const Device& other) = delete;

  Device(Device&& other);
  Device& operator=(Device&& other);

  ~Device()
  {
    if (this->device == VK_NULL_HANDLE)
    {
      return;
    }

    vkDestroyDevice(device, nullptr);
    this->device = VK_NULL_HANDLE;
  }

 private:
  Device(VkPhysicalDevice phys_device, VkDevice device, VkQueue graphics_queue)
      : adapter(phys_device), device(device), graphics_queue(graphics_queue)
  {}

  VkPhysicalDevice adapter;
  VkDevice device;
  VkQueue graphics_queue;
};

struct Renderer
{
  static Result<Renderer, Gfx_Error> new_();
  ~Renderer();

  Renderer(const Renderer& other) = delete;
  Renderer& operator=(const Renderer& other) = delete;

  Renderer(Renderer&& other) : instance(other.instance), device(other.device)
  {
    other.instance = nullptr;
    other.device = std::nullopt;
  }

  void set_device(Device* device) { this->device = device; }
  VkInstance get_instance() const { return this->instance; }

 private:
  Renderer(VkInstance instance) : instance(instance), device(std::nullopt) {}

  VkInstance instance;
  std::optional<Device*> device;
};

}  // namespace nib