#include <unordered_map>
#include <string>

#include "PtVkCommon.h"
#include "PtVulkanResource.h"
#include "PtVulkanContext.h"


static std::unordered_map<size_t, VkFramebuffer> gFrameBuffer;


VkFramebuffer CVulkanContext::PtGetOrCreateVulkanFrameBuffer(const CRHIRenderPassInfo& renderPassInfo, VkRenderPass renderPass)
{
	size_t hash = std::hash<std::string>{}(std::string((const char*)&renderPassInfo, sizeof(CRHIRenderPassInfo)));
	auto iter = gFrameBuffer.find(hash);
	if (iter != gFrameBuffer.end())
	{
		return iter->second;
	}

	CVulkanTexture2D* texture2D = static_cast<CVulkanTexture2D*>(renderPassInfo.rtTexture);
	VkExtent2D actualExtent = { texture2D->m_width,texture2D->m_height };

	VkImageView attachment = texture2D->m_view;
	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass;
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.pAttachments = &attachment;
	framebufferInfo.width = actualExtent.width;
	framebufferInfo.height = actualExtent.height;
	framebufferInfo.layers = 1;

	VkFramebuffer frameBuffer;
	VULKAN_VARIFY(vkCreateFramebuffer(m_device->m_vkDevice, &framebufferInfo, nullptr, &frameBuffer));
	gFrameBuffer[hash] = frameBuffer;
	return frameBuffer;
}


void CVulkanContext::RHIBeginFrame()
{
	vkResetCommandBuffer(*m_vkCmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);
}

void CVulkanContext::RHIEndFrame()
{
	m_device->Present();
}

void CVulkanContext::RHISetGraphicsPipelineState(std::shared_ptr<CRHIGraphicsPipelineState> graphicsPso)
{
}

void CVulkanContext::RHIBeginRenderPass(CRHIRenderPass* rhiRenderPass, CRHITexture2D* rtTextures, uint32_t rtNum)
{
	VkRenderPass renderPass = PtGetOrCreateVulkanRenderPass(rhiRenderPassInfo);
	VkFramebuffer frameBuffer = PtGetOrCreateVulkanFrameBuffer(rhiRenderPassInfo, renderPass);

	CVulkanTexture2D* texture2D = static_cast<CVulkanTexture2D*>(rhiRenderPassInfo.rtTexture);
	VkExtent2D actualExtent = { texture2D->m_width,texture2D->m_height };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = frameBuffer;
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = actualExtent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(*m_vkCmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}
