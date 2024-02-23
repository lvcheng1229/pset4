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
    CreateSwapChain();
    CreateBackBufferImageViews();
    CreateCmdPoolAndCmdBuffer();
    CreateSyncObjects();

    m_gfxCtx = new CVulkanContext(this, &m_commandBuffer);
}

void CVulkanDevice::Present()
{
    m_currentBackBufferIndex++;
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

void CVulkanDevice::CreateSwapChain()
{
    glfwCreateWindowSurface(m_vkInstance, m_glfwWindow, nullptr, &m_surface);

    VkSurfaceCapabilitiesKHR Capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_vkPhysicalDevice, m_surface, &Capabilities);
    uint32_t DesiredNumBuffers = Capabilities.maxImageCount > 0 ? std::clamp(2u, Capabilities.minImageCount, Capabilities.maxImageCount) : 2u;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;
    createInfo.minImageCount = DesiredNumBuffers;
    createInfo.imageFormat = m_swapchainFormat;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = Capabilities.currentExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = Capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VULKAN_VARIFY(vkCreateSwapchainKHR(m_vkDevice, &createInfo, nullptr, &m_swapChain));

    vkGetSwapchainImagesKHR(m_vkDevice, m_swapChain, &DesiredNumBuffers, nullptr);
    m_backBufferImages.resize(DesiredNumBuffers);
    vkGetSwapchainImagesKHR(m_vkDevice, m_swapChain, &DesiredNumBuffers, m_backBufferImages.data());

    m_swapChainExtent = Capabilities.currentExtent;
}

void CVulkanDevice::CreateBackBufferImageViews()
{
    m_swapChainImageViews.resize(m_backBufferImages.size());

    for (size_t i = 0; i < m_backBufferImages.size(); i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_backBufferImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_swapchainFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VULKAN_VARIFY(vkCreateImageView(m_vkDevice, &createInfo, nullptr, &m_swapChainImageViews[i]));
    }

    int width, height;
    glfwGetFramebufferSize(m_glfwWindow, &width, &height);
    VkExtent2D actualExtent = { static_cast<uint32_t>(width),static_cast<uint32_t>(height) };

    m_backBufferTextures.resize(m_backBufferImages.size());
    for (size_t index = 0; index < m_backBufferImages.size(); index++)
    {
        m_backBufferTextures[index]->m_image = m_backBufferImages[index];
        m_backBufferTextures[index]->m_view = m_swapChainImageViews[index];
        m_backBufferTextures[index]->m_width = width;
        m_backBufferTextures[index]->m_height = height;
    }
}

void CVulkanDevice::CreateCmdPoolAndCmdBuffer()
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = m_queueFamilyIndex;

    VULKAN_VARIFY(vkCreateCommandPool(m_vkDevice, &poolInfo, nullptr, &m_commandPool));

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VULKAN_VARIFY(vkAllocateCommandBuffers(m_vkDevice, &allocInfo, &m_commandBuffer));
}

void CVulkanDevice::CreateSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VULKAN_VARIFY(vkCreateSemaphore(m_vkDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore));
    VULKAN_VARIFY(vkCreateSemaphore(m_vkDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore));
    VULKAN_VARIFY(vkCreateFence(m_vkDevice, &fenceInfo, nullptr, &inFlightFence));
}

//void CVulkanDevice::CreateBackBufferFramebuffers()
//{
//    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());
//
//    int width, height;
//    glfwGetFramebufferSize(m_glfwWindow, &width, &height);
//    VkExtent2D actualExtent = { static_cast<uint32_t>(width),static_cast<uint32_t>(height) };
//
//    for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
//    {
//        VkImageView attachments[] = { m_swapChainImageViews[i] };
//
//        VkFramebufferCreateInfo framebufferInfo{};
//        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//        framebufferInfo.renderPass = renderPass;
//        framebufferInfo.attachmentCount = 1;
//        framebufferInfo.pAttachments = attachments;
//        framebufferInfo.width = actualExtent.width;
//        framebufferInfo.height = actualExtent.height;
//        framebufferInfo.layers = 1;
//
//        VULKAN_VARIFY(vkCreateFramebuffer(m_vkDevice, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]));
//    }
//}



