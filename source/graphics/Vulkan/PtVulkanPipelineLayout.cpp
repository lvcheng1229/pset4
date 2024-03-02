#include <string>
#include <xhash>
#include <unordered_map>
#include <assert.h>

#include "PtVkCommon.h"
#include "PtVulkanDynamicRHI.h"

static std::unordered_map<uint32_t, SVulkanPipelineDescSetLayouts> descSetLayoutHash;
static std::unordered_map<uint32_t, VkPipelineLayout> gPipelineLayoutMap;

VkPipelineLayout CVulkanDynamicRHI::PtCreateVulkanGraphicsPipelineLayout(CRHIVertexShader* vertexShader, CRHIPixelShader* pixelShader, SVulkanPipelineDescSetLayouts* outDescSetLayouts)
{
	uint32_t vtxbindingIndex = 0;
	std::vector<VkDescriptorSetLayoutBinding> vtxDescSetLayoutBindings;
	for (uint32_t index = 0; index < vertexShader->m_numCbv; index++)
	{
		VkDescriptorSetLayoutBinding descSetLayoutBinding{};
		descSetLayoutBinding.binding = vtxbindingIndex++;
		descSetLayoutBinding.descriptorCount = 1;
		descSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descSetLayoutBinding.pImmutableSamplers = nullptr;
		descSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		vtxDescSetLayoutBindings.push_back(descSetLayoutBinding);
	}

	assert(pixelShader->m_numCbv == 0);

	uint32_t pixbindingIndex = 0;
	std::vector<VkDescriptorSetLayoutBinding> pixDescSetLayoutBindings;
	for (uint32_t index = 0; index < pixelShader->m_numSrv; index++)
	{
		VkDescriptorSetLayoutBinding descSetLayoutBinding{};
		descSetLayoutBinding.binding = pixbindingIndex++;
		descSetLayoutBinding.descriptorCount = 1;
		descSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		descSetLayoutBinding.pImmutableSamplers = nullptr;
		descSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		pixDescSetLayoutBindings.push_back(descSetLayoutBinding);
	}

	for (uint32_t index = 0; index < pixelShader->m_numSampler; index++)
	{
		VkDescriptorSetLayoutBinding descSetLayoutBinding{};
		descSetLayoutBinding.binding = pixbindingIndex++;
		descSetLayoutBinding.descriptorCount = 1;
		descSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		descSetLayoutBinding.pImmutableSamplers = nullptr;
		descSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		pixDescSetLayoutBindings.push_back(descSetLayoutBinding);
	}

	uint32_t bidingCount = pixbindingIndex + vtxbindingIndex;

	std::vector<VkPushConstantRange> m_pushConstantRages;
	m_pushConstantRages.resize(pixelShader->m_numPushConst);
	for (uint32_t index = 0; index < pixelShader->m_numPushConst; index++)
	{
		m_pushConstantRages[index] = {};
		m_pushConstantRages[index].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		m_pushConstantRages[index].offset = 0;
		m_pushConstantRages[index].size = pixelShader->m_pushConstSizes[index];
	}

	std::vector<VkDescriptorSetLayout> descSetLayouts;
	
	size_t hash = 42;
	if (bidingCount > 0 || m_pushConstantRages.size() > 0)
	{
		if (vtxbindingIndex > 0)
		{
			size_t vtxHash = std::hash<std::string>{}(std::string((char*)vtxDescSetLayoutBindings.data(), sizeof(VkDescriptorSetLayoutBinding) * vtxDescSetLayoutBindings.size()));
			THashCombine(hash, vtxHash);
		}
		
		if (pixbindingIndex > 0)
		{
			size_t pixHash = std::hash<std::string>{}(std::string((char*)pixDescSetLayoutBindings.data(), sizeof(VkDescriptorSetLayoutBinding) * pixDescSetLayoutBindings.size()));
			THashCombine(hash, pixHash);
		}

		if (m_pushConstantRages.size() > 0)
		{
			for (uint32_t index = 0; index < pixelShader->m_numPushConst; index++)
			{
				THashCombine(hash, pixelShader->m_pushConstSizes[index]);
			}
		}

		auto iter = gPipelineLayoutMap.find(hash);
		if (iter == gPipelineLayoutMap.end())
		{
			if (vtxbindingIndex > 0)
			{
				VkDescriptorSetLayout vtxDescSetLayout;
				VkDescriptorSetLayoutCreateInfo vtxDescriptorLayoutInfo = {};
				vtxDescriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				vtxDescriptorLayoutInfo.bindingCount = vtxDescSetLayoutBindings.size();
				vtxDescriptorLayoutInfo.pBindings = vtxDescSetLayoutBindings.data();
				vkCreateDescriptorSetLayout(m_device.m_vkDevice, &vtxDescriptorLayoutInfo, nullptr, &vtxDescSetLayout);
				descSetLayoutHash[hash].vtxDescSetLayout = vtxDescSetLayout;
				descSetLayoutHash[hash].bVtxHasBind = true;
				descSetLayouts.push_back(vtxDescSetLayout);
			}

			if (pixbindingIndex > 0)
			{
				VkDescriptorSetLayout pixDescSetLayout;
				VkDescriptorSetLayoutCreateInfo pixDescriptorLayoutInfo = {};
				pixDescriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
				pixDescriptorLayoutInfo.bindingCount = pixDescSetLayoutBindings.size();
				pixDescriptorLayoutInfo.pBindings = pixDescSetLayoutBindings.data();
				vkCreateDescriptorSetLayout(m_device.m_vkDevice, &pixDescriptorLayoutInfo, nullptr, &pixDescSetLayout);
				descSetLayoutHash[hash].pixDescSetLayout = pixDescSetLayout;
				descSetLayoutHash[hash].bPixHasBind = true;
				descSetLayouts.push_back(pixDescSetLayout);
			}

			*outDescSetLayouts = descSetLayoutHash[hash];
		}
		else
		{
			*outDescSetLayouts = descSetLayoutHash[hash];
			return gPipelineLayoutMap[hash];
		}
	}

	VkPipelineLayout pipelineLayout;
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = descSetLayouts.size();
	pipelineLayoutCreateInfo.pSetLayouts = descSetLayouts.data();
	pipelineLayoutCreateInfo.pushConstantRangeCount = pixelShader->m_numPushConst;
	pipelineLayoutCreateInfo.pPushConstantRanges = m_pushConstantRages.data();
	VULKAN_VARIFY(vkCreatePipelineLayout(m_device.m_vkDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));
	gPipelineLayoutMap[hash] = pipelineLayout;
	return pipelineLayout;
}


