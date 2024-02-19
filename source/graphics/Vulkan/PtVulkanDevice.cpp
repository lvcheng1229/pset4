#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core\Log.h"

#include "PtVkCommon.h"
#include "PtVulkanDevice.h"

void CVulkanDevice::Init(void* windowHandle)
{
	m_glfwWindow = (GLFWwindow*)windowHandle;
	InitVulkanInstance();
    PickPhysicalDevice();
    CreateLogicalDevice();
}

const std::vector<const char*>gValidationLayers = { "VK_LAYER_KHRONOS_validation" };
const std::vector<const char*>gDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

std::vector<const char*> GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
#if PSET_DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return extensions;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VkDefaultDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    PSET_LOG_ERROR("Vulkan Validation Layer:" + std::string(pCallbackData->pMessage));
    return VK_FALSE;
}

void CVulkanDevice::InitVulkanInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Pset4";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vk Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto insExtensions = GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(insExtensions.size());;
    createInfo.ppEnabledExtensionNames = insExtensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
#if PSET_DEBUG
    debugCreateInfo = {};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = VkDefaultDebugCallback;

    createInfo.enabledLayerCount = static_cast<uint32_t>(gValidationLayers.size());
    createInfo.ppEnabledLayerNames = gValidationLayers.data();
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

#else 
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
#endif

    std::vector<const char*> insLayers;
    VULKAN_VARIFY(vkCreateInstance(&createInfo, nullptr, &m_vkInstance));
}

static bool IsDeviceSuitable(VkPhysicalDevice device)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (const auto& queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            return true;
        }
    }
    return false;
}

void CVulkanDevice::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);
    PSET_EXIT_AND_LOG_IF(deviceCount == 0, "failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());

    for (const auto& device : devices)
    {
        if (IsDeviceSuitable(device)) { m_vkPhysicalDevice = device; break; }
    }

    PSET_EXIT_AND_LOG_IF(m_vkPhysicalDevice == VK_NULL_HANDLE, "failed to find a suitable GPU!");
}

void CVulkanDevice::CreateLogicalDevice()
{
        uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

    int32_t GfxQueueFamilyIndex = -1;
    for (int32_t Index = 0; Index < queueFamilies.size(); Index++)
    {
        if (queueFamilies[Index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            GfxQueueFamilyIndex = Index;
            break;
        }
    }

    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = GfxQueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(gDeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = gDeviceExtensions.data();

#if PSET_DEBUG
    createInfo.enabledLayerCount = static_cast<uint32_t>(gValidationLayers.size());
    createInfo.ppEnabledLayerNames = gValidationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    VULKAN_VARIFY(vkCreateDevice(m_vkPhysicalDevice, &createInfo, nullptr, &m_vkDevice));
    vkGetDeviceQueue(m_vkDevice, GfxQueueFamilyIndex, 0, &m_vkQueue);

    m_queueFamilyIndex = GfxQueueFamilyIndex;
}



