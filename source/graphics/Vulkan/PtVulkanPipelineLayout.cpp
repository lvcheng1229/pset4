#include <string>
#include <xhash>
#include <unordered_map>

#include "PtVkCommon.h"
#include "PtVulkanDynamicRHI.h"

static std::unordered_map<uint32_t, VkDescriptorSetLayout> descSetLayoutHash;


VkPipelineLayout CVulkanDynamicRHI::PtCreateVulkanGraphicsPipelineLayout(CRHIVertexShader* vertexShader, CRHIPixelShader* pixelShader)
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

	for (uint32_t index = 0; index < pixelShader->m_numCbv; index++)
	{
		VkDescriptorSetLayoutBinding descSetLayoutBinding{};
		descSetLayoutBinding.binding = bindingIndex++;
		descSetLayoutBinding.descriptorCount = 1;
		descSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descSetLayoutBinding.pImmutableSamplers = nullptr;
		descSetLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		descSetLayoutBindings.push_back(descSetLayoutBinding);
	}

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

	for (uint32_t index = 0; index < pixelShader->m_numSrv; index++)
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

	VkDescriptorSetLayout descSetLayout;
	if (bidingCount > 0)
	{
		size_t hash = std::hash<std::string>{}(std::string((char*)descSetLayoutBindings.data(), sizeof(VkDescriptorSetLayoutBinding) * descSetLayoutBindings.size()));
		auto iter = descSetLayoutHash.find(hash);
		if (iter == descSetLayoutHash.end())
		{
			VkDescriptorSetLayoutCreateInfo DescriptorLayoutInfo = {};
			DescriptorLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			DescriptorLayoutInfo.bindingCount = descSetLayoutBindings.size();
			DescriptorLayoutInfo.pBindings = descSetLayoutBindings.data();
			vkCreateDescriptorSetLayout(m_device.m_vkDevice, &DescriptorLayoutInfo, nullptr, &descSetLayout);
		}
		else
		{
			descSetLayout = iter->second;
		}
	}

	VkPipelineLayout pipelineLayout;
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = bidingCount > 0 ? 1 : 0;
	pipelineLayoutCreateInfo.pSetLayouts = &descSetLayout;
	VULKAN_VARIFY(vkCreatePipelineLayout(m_device.m_vkDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));
	return pipelineLayout;
}


