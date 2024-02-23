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

VkRenderPass CVulkanDynamicRHI::PtCreateVulkanRenderPass(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc)
{
	std::vector<VkAttachmentReference>colorAttachmentRefs;
	for (uint32_t index = 0; index < psoInitDesc.m_rtNum; index++)
	{
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachmentRefs.push_back(colorAttachmentRef);
	}

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = psoInitDesc.m_rtNum;
	subpass.pColorAttachments = colorAttachmentRefs.data();

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkAttachmentDescription attachmentDesc[8];
	for (uint32_t index = 0; index < psoInitDesc.m_rtNum; index++)
	{
		attachmentDesc[index] = {};
		attachmentDesc[index].format = GetVkRenderTargetFormatFromAMDFormat(psoInitDesc.renderTargets[index].INFO.bitfields.FORMAT, psoInitDesc.renderTargets[index].INFO.bitfields.NUMBER_TYPE);
		attachmentDesc[index].samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDesc[index].loadOp = psoInitDesc.renderTargets[index].INFO.bitfields.FAST_CLEAR != 0 ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
		attachmentDesc[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDesc[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDesc[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDesc[index].initialLayout = VK_IMAGE_LAYOUT_GENERAL;
		attachmentDesc[index].finalLayout = VK_IMAGE_LAYOUT_GENERAL;
	}

	uint32_t totalRtNum = psoInitDesc.m_rtNum;
	if (psoInitDesc.m_dsState.bDepthTestEnable || psoInitDesc.m_dsState.bDepthWriteEnable)
	{
		totalRtNum++;

		uint32_t dsIndex = psoInitDesc.m_rtNum;
		attachmentDesc[dsIndex].format = VK_FORMAT_D24_UNORM_S8_UINT;
		attachmentDesc[dsIndex].samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDesc[dsIndex].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmentDesc[dsIndex].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDesc[dsIndex].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDesc[dsIndex].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDesc[dsIndex].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		attachmentDesc[dsIndex].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = totalRtNum;
	renderPassInfo.pAttachments = attachmentDesc;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VkRenderPass renderPass;
	VULKAN_VARIFY(vkCreateRenderPass(m_device.m_vkDevice, &renderPassInfo, nullptr, &renderPass));
	return renderPass;
}
