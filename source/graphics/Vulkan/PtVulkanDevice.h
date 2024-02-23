#pragma once
#include <vector>
#include "PtVulkanContext.h"
#include "PtVulkanResource.h"

#include "vulkan\vulkan.h"


class CVulkanDevice
{
public:
	void Init(void* windowHandle);
	inline CVulkanContext* GetContext() { return m_gfxCtx; }
	std::shared_ptr<CVulkanTexture2D> GetCurrentBackBufferTexture() { return m_backBufferTextures[m_currentBackBufferIndex]; };
	void Present();
public:
	VkDevice m_vkDevice;
private:
	void InitVulkanInstance();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapChain();
	void CreateBackBufferImageViews();
	//void CreateBackBufferFramebuffers();
	void CreateCmdPoolAndCmdBuffer();
	void CreateSyncObjects();

private:
	struct GLFWwindow* m_glfwWindow;
	
	VkInstance m_vkInstance;
	VkPhysicalDevice m_vkPhysicalDevice;	
	VkQueue m_vkQueue;	
	uint32_t m_queueFamilyIndex;

	// commnad pool
	VkCommandPool m_commandPool;
	VkCommandBuffer m_commandBuffer;

	// Sync objects
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	// swap chain
	VkExtent2D m_swapChainExtent;
	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapChain;
	VkFormat m_swapchainFormat = VK_FORMAT_B8G8R8A8_SRGB;

	uint32_t m_currentBackBufferIndex = 0;
	std::vector<std::shared_ptr<CVulkanTexture2D>> m_backBufferTextures;
	std::vector<VkImage>m_backBufferImages;
	std::vector<VkImageView> m_swapChainImageViews;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;

	// contex
	CVulkanContext* m_gfxCtx;
};