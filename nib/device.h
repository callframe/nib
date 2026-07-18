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

  ~Device() { vkDestroyDevice(device, nullptr); }

 private:
  Device(VkPhysicalDevice phys_device, VkDevice device, VkQueue graphics_queue,
         std::uint32_t graphics_queue_family)
      : adapter(phys_device),
        device(device),
        device_queue(graphics_queue),
        device_queue_family(graphics_queue_family)
  {}

  VkPhysicalDevice adapter;
  VkDevice device;
  VkQueue device_queue;
  std::uint32_t device_queue_family;
};

}  // namespace nib
