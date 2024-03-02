#include <array>
#include "PtVkCommon.h"
#include "PtVulkanDescSetManager.h"

void CVulkanDescSetManager::Init(VkDevice vkDevice)
{
    static constexpr uint32_t maxDescCount = 1024;
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = maxDescCount;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = maxDescCount;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = 128;

    m_vkDevice = vkDevice;

    VULKAN_VARIFY(vkCreateDescriptorPool(m_vkDevice, &poolInfo, nullptr, &m_descPool));
}

VkDescriptorSet CVulkanDescSetManager::PtAllocateDescriptorSets(VkDescriptorSetLayout* descSetLayout)
{
    VkDescriptorSet pixDescSet;
    VkDescriptorSetAllocateInfo pixAllocInfo{};
    pixAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    pixAllocInfo.descriptorPool = m_descPool;
    pixAllocInfo.descriptorSetCount = 1;
    pixAllocInfo.pSetLayouts = descSetLayout;
    vkAllocateDescriptorSets(m_vkDevice, &pixAllocInfo, &pixDescSet);
    m_descSets.push_back(pixDescSet);
	return pixDescSet;
}

void CVulkanDescSetManager::ResetDescSetPool()
{
    if (m_descSets.size() > 0)
    {
        vkResetDescriptorPool(m_vkDevice, m_descPool, 0);
        m_descSets.resize(0);
    }
}
