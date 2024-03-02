#include <vector>
#include <locale>
#include <codecvt>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core\Log.h"

#include "PtVkCommon.h"
#include "PtVulkanDevice.h"

#define ENABLE_VALIDATION_LAYER 1

class CPtRenderDocAPI
{
public:
    CPtRenderDocAPI();
    RENDERDOC_API_1_6_0* m_rdoc;
};

static CPtRenderDocAPI rdocApi;

CPtRenderDocAPI::CPtRenderDocAPI()
{
    RENDERDOC_API_1_6_0* rdoc = nullptr;
    std::string rdocPath = std::string(PSET_ROOT_DIR) + "/thirdparty/renderdoc/renderdoc.dll";
    std::wstring rdocWPath = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(rdocPath.c_str());
   
    HMODULE module = GetModuleHandleA("renderdoc.dll");
    if (module == NULL)
    {
        return;
    }

    pRENDERDOC_GetAPI getApi = nullptr;
    getApi = (pRENDERDOC_GetAPI)GetProcAddress(module, "RENDERDOC_GetAPI");

    if (getApi == nullptr)
    {
        return;
    }

    if (getApi(eRENDERDOC_API_Version_1_6_0, (void**)&rdoc) != 1)
    {
        return;
    }

    m_rdoc =  rdoc;
}

void CVulkanDevice::Init(void* windowHandle)
{
    m_glfwWindow = (GLFWwindow*)windowHandle;
    m_rdoc = rdocApi.m_rdoc;
    InitVulkanInstance();
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateSwapChain();
    CreateBackBufferImageViews();
    CreateCmdPoolAndCmdBuffer();
    CreateSyncObjects();
    CreateAmdVulkanMemAllocator();
    CreateDeviceDefaultDepthTexture();
    m_descManager.Init(m_vkDevice);
    CreateStaticSampler();

    if (m_rdoc)
    {
        m_rdoc->StartFrameCapture(nullptr, nullptr);
    }
    
    m_gfxCtx = new CVulkanContext(this, &m_commandBuffer);
}

void CVulkanDevice::Present()
{
    m_currentBackBufferIndex++;

    vkAcquireNextImageKHR(m_vkDevice, m_swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &m_acquiredImageIndex);


    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer;

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    VULKAN_VARIFY(vkQueueSubmit(m_vkQueue, 1, &submitInfo, inFlightFence));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { m_swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &m_acquiredImageIndex;

    vkQueuePresentKHR(m_vkQueue, &presentInfo);
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

#if ENABLE_VALIDATION_LAYER
    createInfo.enabledLayerCount = static_cast<uint32_t>(gValidationLayers.size());
    createInfo.ppEnabledLayerNames = gValidationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
#endif
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

#if ENABLE_VALIDATION_LAYER
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

    glfwGetFramebufferSize(m_glfwWindow, &m_width, &m_height);

    m_backBufferTextures.resize(m_backBufferImages.size());
    for (size_t index = 0; index < m_backBufferImages.size(); index++)
    {
        m_backBufferTextures[index] = std::make_shared<CVulkanTexture2D>();
        m_backBufferTextures[index]->m_width = m_width;
        m_backBufferTextures[index]->m_height = m_height;
        m_backBufferTextures[index]->m_image = m_backBufferImages[index];
        m_backBufferTextures[index]->m_view = m_swapChainImageViews[index];
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

void CVulkanDevice::CreateAmdVulkanMemAllocator()
{
    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_1;
    allocatorCreateInfo.physicalDevice = m_vkPhysicalDevice;
    allocatorCreateInfo.device = m_vkDevice;
    allocatorCreateInfo.instance = m_vkInstance;
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    vmaCreateAllocator(&allocatorCreateInfo, &m_vmaAllocator);
}

void CVulkanDevice::CreateDeviceDefaultDepthTexture()
{
    VkImageCreateInfo imgCreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
    imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imgCreateInfo.extent.width = m_width;
    imgCreateInfo.extent.height = m_height;
    imgCreateInfo.extent.depth = 1;
    imgCreateInfo.mipLevels = 1;
    imgCreateInfo.arrayLayers = 1;
    imgCreateInfo.format = VK_FORMAT_D24_UNORM_S8_UINT;
    imgCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    //imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    imgCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    allocCreateInfo.priority = 1.0f;

    m_deviceDefaultDsTexture = std::make_shared<CVulkanTexture2D>();
    m_deviceDefaultDsTexture->m_width = m_width;
    m_deviceDefaultDsTexture->m_height = m_height;

    vmaCreateImage(m_vmaAllocator, &imgCreateInfo, &allocCreateInfo, &m_deviceDefaultDsTexture->m_image, &m_deviceDefaultDsTexture->m_vmaAlloc, nullptr);

    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = m_deviceDefaultDsTexture->m_image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = VK_FORMAT_D24_UNORM_S8_UINT;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;
    VULKAN_VARIFY(vkCreateImageView(m_vkDevice, &createInfo, nullptr, &m_deviceDefaultDsTexture->m_view));
}


void CVulkanDevice::CreateStaticSampler()
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

    vkCreateSampler(m_vkDevice, &samplerInfo, nullptr, &m_vkStaticPointSampler);
}

