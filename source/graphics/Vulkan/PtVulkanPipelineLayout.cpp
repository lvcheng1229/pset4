#include <string>
#include <xhash>
#include <unordered_map>
#include <assert.h>

#include "PtVkCommon.h"
#include "PtVulkanDynamicRHI.h"

static std::unordered_map<uint32_t, VkDescriptorSetLayout> descSetLayoutHash;
static std::unordered_map<uint32_t, VkDescriptorSet> gDescSetMap;

VkPipelineLayout CVulkanDynamicRHI::PtCreateVulkanGraphicsPipelineLayout(CRHIVertexShader* vertexShader, CRHIPixelShader* pixelShader, VkDescriptorSet* outDescSet)
{
	std::vector<VkDescriptorSetLayoutBinding> descSetLayoutBindings;
	uint32_t bindingIndex = 0;
	for (uint32_t index = 0; index < vertexShader->m_numCbv; index++)
	{
		VkDescriptorSetLayoutBinding descSetLayoutBinding{};
		descSetLayoutBinding.binding = bindingIndex++;
		descSetLayoutBinding.descriptorCount = 1;
		descSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descSetLayoutBinding.pImmutableSamplers = nullptr;
		descSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		descSetLayoutBindings.push_back(descSetLayoutBinding);
	}

	assert(pixelShader->m_numCbv == 0);

	for (uint32_t index = 0; index < pixelShader->m_numSrv; index++)
	{
		VkDescriptorSetLayoutBinding descSetLayoutBinding{};
		descSetLayoutBinding.binding = bindingIndex++;
		descSetLayoutBinding.descriptorCount = 1;
		descSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		descSetLayoutBinding.pImmutableSamplers = nullptr;
		descSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		descSetLayoutBindings.push_back(descSetLayoutBinding);
	}

	for (uint32_t index = 0; index < pixelShader->m_numSampler; index++)
	{
		VkDescriptorSetLayoutBinding descSetLayoutBinding{};
		descSetLayoutBinding.binding = bindingIndex++;
		descSetLayoutBinding.descriptorCount = 1;
		descSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		descSetLayoutBinding.pImmutableSamplers = nullptr;
		descSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		descSetLayoutBindings.push_back(descSetLayoutBinding);
	}

	uint32_t bidingCount = descSetLayoutBindings.size();

	std::vector<VkPushConstantRange> m_pushConstantRages;
	m_pushConstantRages.resize(pixelShader->m_numPushConst);
	for (uint32_t index = 0; index < pixelShader->m_numPushConst; index++)
	{
		m_pushConstantRages[index] = {};
		m_pushConstantRages[index].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		m_pushConstantRages[index].offset = 0;
		m_pushConstantRages[index].size = pixelShader->m_pushConstSizes[index];
	}

	VkDescriptorSetLayout descSetLayout;
	if (bidingCount > 0 || m_pushConstantRages.size() > 0)
	{
		size_t hash = std::hash<std::string>{}(std::string((char*)descSetLayoutBindings.data(), sizeof(VkDescriptorSetLayoutBinding) * descSetLayoutBindings.size()));
		
		if (m_pushConstantRages.size() > 0)
		{
			for (uint32_t index = 0; index < pixelShader->m_numPushConst; index++)
			{
				THashCombine(hash, pixelShader->m_pushConstSizes[index]);
			}
		}

		auto iter = descSetLayoutHash.find(hash);
		if (iter == descSetLayoutHash.end())
		{
			VkDescriptorSetLayoutCreateInfo DescriptorLayoutInfo = {};
			DescriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			DescriptorLayoutInfo.bindingCount = descSetLayoutBindings.size();
			DescriptorLayoutInfo.pBindings = descSetLayoutBindings.data();
			vkCreateDescriptorSetLayout(m_device.m_vkDevice, &DescriptorLayoutInfo, nullptr, &descSetLayout);
			descSetLayoutHash[hash] = descSetLayout;
			
			VkDescriptorSet vkDescSet;
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = m_device.m_descPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &descSetLayout;
			vkAllocateDescriptorSets(m_device.m_vkDevice, &allocInfo, &vkDescSet);
			gDescSetMap[hash] = vkDescSet;
			*outDescSet = gDescSetMap[hash];
		}
		else
		{
			descSetLayout = iter->second;
			*outDescSet = gDescSetMap[hash];
		}
	}



	VkPipelineLayout pipelineLayout;
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = bidingCount > 0 ? 1 : 0;
	pipelineLayoutCreateInfo.pSetLayouts = &descSetLayout;
	pipelineLayoutCreateInfo.pushConstantRangeCount = pixelShader->m_numPushConst;
	pipelineLayoutCreateInfo.pPushConstantRanges = m_pushConstantRages.data();
	VULKAN_VARIFY(vkCreatePipelineLayout(m_device.m_vkDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));
	return pipelineLayout;
}


