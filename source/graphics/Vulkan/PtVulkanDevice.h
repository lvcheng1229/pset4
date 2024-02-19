#pragma once
#include "vulkan\vulkan.h"

class CVulkanDevice
{
public:
	void Init(void* windowHandle);
public:
	VkDevice m_vkDevice;
private:
	void InitVulkanInstance();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
private:
	struct GLFWwindow* m_glfwWindow;
	VkInstance m_vkInstance;
	VkPhysicalDevice m_vkPhysicalDevice;
	VkQueue m_vkQueue;
	uint32_t m_queueFamilyIndex;
};