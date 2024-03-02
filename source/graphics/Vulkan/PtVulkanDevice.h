#pragma once
#include <vector>

#include "PtVulkanDescSetManager.h"
#include "PtVulkanContext.h"
#include "PtVulkanResource.h"

#include "vulkan\vulkan.h"

#include "renderdoc_app.h"

#include "vk_mem_alloc.h"

class CVulkanDevice
{
	friend class CVulkanDynamicRHI;
	friend class CVulkanContext;
public:
	void Init(void* windowHandle);
	inline CVulkanContext* GetContext() { return m_gfxCtx; }
	inline std::shared_ptr<CVulkanTexture2D> GetCurrentBackBufferTexture() { return m_backBufferTextures[m_currentBackBufferIndex]; };
	inline std::shared_ptr<CVulkanTexture2D> GetDefaultDSTexture() { return m_deviceDefaultDsTexture; };
	
	inline RENDERDOC_API_1_6_0* GetRenderDocAPI() { return m_rdoc; };

	void Present();
public:
	VkDevice m_vkDevice;
private:
	void InitVulkanInstance();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapChain();
	void CreateBackBufferImageViews();
	void CreateCmdPoolAndCmdBuffer();
	void CreateSyncObjects();
	void CreateAmdVulkanMemAllocator();
	void CreateDeviceDefaultDepthTexture();
	void CreateStaticSampler();

private:
	struct GLFWwindow* m_glfwWindow;
	
	VkInstance m_vkInstance;
	VkPhysicalDevice m_vkPhysicalDevice;	
	VkQueue m_vkQueue;	
	uint32_t m_queueFamilyIndex;

	// 
	VkSampler m_vkStaticPointSampler;

	// vulkan descriptor manager
	CVulkanDescSetManager m_descManager;

	// renderdoc
	RENDERDOC_API_1_6_0* m_rdoc;

	// commnad pool
	VkCommandPool m_commandPool;
	VkCommandBuffer m_commandBuffer;

	// amd vulkan allocator
	VmaAllocator m_vmaAllocator;

	// Sync objects
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	// window size
	int32_t m_width;
	int32_t m_height;

	// device default depth texture
	std::shared_ptr<CVulkanTexture2D> m_deviceDefaultDsTexture;

	uint32_t m_acquiredImageIndex;

	// swap chain
	VkExtent2D m_swapChainExtent;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapChain;
	VkFormat m_swapchainFormat = VK_FORMAT_R8G8B8A8_UNORM;

	uint32_t m_currentBackBufferIndex = 0;
	std::vector<std::shared_ptr<CVulkanTexture2D>> m_backBufferTextures;
	std::vector<VkImage>m_backBufferImages;
	std::vector<VkImageView> m_swapChainImageViews;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;

	// contex
	CVulkanContext* m_gfxCtx;
};