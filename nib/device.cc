#include "nib/device.h"

#include <vulkan/vulkan_core.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "nib/common.h"

constexpr static float QUEUE_PRIORITY = 1.0f;

constexpr char const* EXTENSIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

static std::size_t device_score(VkPhysicalDevice adapter)
{
  std::size_t score = 50;

  VkPhysicalDeviceProperties props;
  vkGetPhysicalDeviceProperties(adapter, &props);

  switch (props.deviceType)
  {
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
      score = 300;
      break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
      score = 200;
      break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
      score = 100;
      break;
    default:
      score = 0;
      break;
  }

  return score;
}

static bool device_get_adapter(VkInstance instance, VkPhysicalDevice* out_adapter)
{
  std::uint32_t adapter_count = 0;
  vkEnumeratePhysicalDevices(instance, &adapter_count, nullptr);
  if (adapter_count == 0)
  {
    std::fprintf(stderr, "Failed to find GPUs with Vulkan support\n");
    return false;
  }

  std::vector<VkPhysicalDevice> adapters(adapter_count);
  vkEnumeratePhysicalDevices(instance, &adapter_count, adapters.data());

  VkPhysicalDevice best_adapter = VK_NULL_HANDLE;
  std::size_t best_score = 0;
  for (VkPhysicalDevice adapter : adapters)
  {
    std::size_t score = device_score(adapter);
    if (score > best_score)
    {
      best_score = score;
      best_adapter = adapter;
    }
  }

  *out_adapter = best_adapter;
  return true;
}

static bool device_score_queue_family(VkPhysicalDevice adapter, VkSurfaceKHR surface,
                                      VkQueueFamilyProperties const& family,
                                      std::uint32_t family_index)
{
  VkBool32 present_support = VK_FALSE;
  vkGetPhysicalDeviceSurfaceSupportKHR(adapter, family_index, surface, &present_support);

  return present_support && (family.queueFlags & VK_QUEUE_GRAPHICS_BIT);
}

static bool device_select_queue_family(VkPhysicalDevice adapter, VkSurfaceKHR surface,
                                       std::uint32_t* out_family_index)
{
  std::uint32_t family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(adapter, &family_count, nullptr);
  if (family_count == 0)
  {
    std::fprintf(stderr, "Failed to find queue families for Vulkan device\n");
    return false;
  }

  std::vector<VkQueueFamilyProperties> families(family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(adapter, &family_count, families.data());

  for (std::uint32_t i = 0; i < family_count; ++i)
  {
    if (!device_score_queue_family(adapter, surface, families[i], i))

    {
      continue;
    }

    *out_family_index = i;
    return true;
  }

  std::fprintf(stderr, "Failed to find a suitable queue family for Vulkan device\n");
  return false;
}

static VkDeviceQueueCreateInfo device_get_queue_create_info(std::uint32_t family_index)
{
  VkDeviceQueueCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  create_info.queueFamilyIndex = family_index;
  create_info.queueCount = 1;
  create_info.pQueuePriorities = &QUEUE_PRIORITY;

  return create_info;
}

static VkDeviceCreateInfo device_get_create_info(VkDeviceQueueCreateInfo const& queue_create_info)
{
  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.queueCreateInfoCount = 1;
  create_info.pQueueCreateInfos = &queue_create_info;
  create_info.enabledExtensionCount = NIB_ARRAY_LENGTH(EXTENSIONS);
  create_info.ppEnabledExtensionNames = EXTENSIONS;

  return create_info;
}

nib::Device::Device(Device&& other)
    : adapter(other.adapter), device(other.device), graphics_queue(other.graphics_queue)
{
  other.adapter = VK_NULL_HANDLE;
  other.device = VK_NULL_HANDLE;
  other.graphics_queue = VK_NULL_HANDLE;
}

nib::Device& nib::Device::operator=(Device&& other)
{
  if (this != &other)
  {
    this->~Device();
    new (this) Device(std::move(other));
  }
  return *this;
}

nib::Result<nib::Device, nib::Device_Error> nib::Device::new_(VkInstance instance,
                                                              VkSurfaceKHR surface)
{
  VkPhysicalDevice adapter;
  if (!device_get_adapter(instance, &adapter))
  {
    return Device_Error::Failed_To_Get_Adapter;
  }

  std::uint32_t queue_family_index;
  if (!device_select_queue_family(adapter, surface, &queue_family_index))
  {
    return Device_Error::Failed_To_Get_Queue_Family;
  }

  VkDeviceQueueCreateInfo queue_create_info = device_get_queue_create_info(queue_family_index);
  VkDeviceCreateInfo device_create_info = device_get_create_info(queue_create_info);

  VkDevice device;
  VkResult result = vkCreateDevice(adapter, &device_create_info, nullptr, &device);
  if (result != VK_SUCCESS)
  {
    std::fprintf(stderr, "Failed to create Vulkan device: %d\n", result);
    return Device_Error::Failed_To_Get_Device;
  }

  VkQueue graphics_queue;
  vkGetDeviceQueue(device, queue_family_index, 0, &graphics_queue);

  return Device(adapter, device, graphics_queue);
}