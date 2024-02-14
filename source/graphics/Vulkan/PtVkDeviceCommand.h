#pragma once
#include <vector>
#include "vulkan\vulkan.h"
#include "graphics\RHI\RHIDeviceCommand.h"

class CPtVkDeviceCommand : public CRHIDeviceCommand
{
public:
	void Init(void* windowHandle) override;
	void TestMainLoop() override;
private:
	void InitVulkanInstance();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffer();
	void CreateSyncObjects();
	
	void DrawFrame();
	void RecordCommandBuffer(uint32_t imageIndex);

	uint32_t m_queueFamilyIndex;

	VkInstance instance;
	VkSurfaceKHR m_surface;

	VkPhysicalDevice m_physicalDevice;
	VkDevice m_vkDevice;
	VkQueue m_vkQueue;

	VkSwapchainKHR m_swapChain;

	std::vector<VkImage>m_backBufferImages;
	std::vector<VkImageView> m_swapChainImageViews;
	std::vector<VkFramebuffer> m_swapChainFramebuffers;
	VkExtent2D m_swapChainExtent;

	VkFormat m_swapchainFormat = VK_FORMAT_B8G8R8A8_SRGB;

	struct GLFWwindow* m_glfwWindow;

	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	bool bInit = false;
};