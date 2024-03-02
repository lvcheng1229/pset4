#pragma once
#include <vector>
#include "vulkan\vulkan.h"

class CVulkanDescSetManager
{
public:
	void Init(VkDevice vkDevice);
	VkDescriptorSet PtAllocateDescriptorSets(VkDescriptorSetLayout* descSetLayout);
	void ResetDescSetPool();
private:
	VkDevice m_vkDevice;
	std::vector<VkDescriptorSet> m_descSets;
	VkDescriptorPool m_descPool;
};