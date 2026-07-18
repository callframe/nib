#pragma once

#include <vulkan/vulkan.h>

#include "nib/result.h"

namespace nib
{

enum class Device_Error
{
  Failed_To_Get_Adapter,
  Failed_To_Get_Queue_Family,
  Failed_To_Get_Device,
};

struct Device
{
  static Result<Device, Device_Error> new_(VkInstance instance, VkSurfaceKHR surface);

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

}  // namespace nib
