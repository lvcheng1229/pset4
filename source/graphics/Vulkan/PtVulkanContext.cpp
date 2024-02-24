#include <unordered_map>
#include <string>
#include <assert.h>

#include "PtVulkanDevice.h"
#include "PtVkCommon.h"
#include "PtVulkanResource.h"
#include "PtVulkanContext.h"

static std::unordered_map<size_t, VkFramebuffer> gFrameBuffer;

VkFramebuffer CVulkanContext::PtGetOrCreateVulkanFrameBuffer(const CVulkanTexture2D* pVkRtTexture2D, VkRenderPass renderPass, const CVulkanTexture2D* pVkDsTexture2D)
{
	VkExtent2D actualExtent = { pVkRtTexture2D->m_width,pVkRtTexture2D->m_height };

	VkImageView attachments[2] = { pVkRtTexture2D->m_view ,pVkDsTexture2D->m_view };

	size_t hash = std::hash<std::string>{}(std::string((const char*)attachments, sizeof(VkImageView) * 2));
	auto iter = gFrameBuffer.find(hash);
	if (iter != gFrameBuffer.end())
	{
		return iter->second;
	}

	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass;
	framebufferInfo.attachmentCount = 2;//tempCode
	framebufferInfo.pAttachments = attachments;
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
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vkResetCommandBuffer(*m_vkCmdBuffer, /*VkCommandBufferResetFlagBits*/ 0);
	vkBeginCommandBuffer(*m_vkCmdBuffer, &beginInfo);
}

void CVulkanContext::RHIEndFrame()
{
	m_device->Present();
}

void CVulkanContext::RHISetGraphicsPipelineState(std::shared_ptr<CRHIGraphicsPipelineState> graphicsPso)
{
	CVulkanGraphicsPipelineState* vkgGraphicsPso = static_cast<CVulkanGraphicsPipelineState*>(graphicsPso.get());
	if (m_gfxStateCache.m_pipeline != vkgGraphicsPso->m_vkPipeline)
	{
		m_gfxStateCache.m_pipeline = vkgGraphicsPso->m_vkPipeline;
		m_gfxStateCache.m_bPipelineDirty = true;
	}
	
}

void CVulkanContext::RHIBeginRenderPass(CRHIRenderPass* rhiRenderPass, CRHITexture2D* rtTextures, uint32_t rtNum, CRHITexture2D* dsTexture)
{
	assert(rtNum == 1);

	CVulkanRenderPass* pVkRenderPass = static_cast<CVulkanRenderPass*>(rhiRenderPass);

	CVulkanTexture2D* pVkTexture = static_cast<CVulkanTexture2D*>(rtTextures);
	VkRenderPass renderPass = pVkRenderPass->m_vkRenderPass;
	VkFramebuffer frameBuffer = PtGetOrCreateVulkanFrameBuffer(pVkTexture, renderPass, static_cast<CVulkanTexture2D*>(dsTexture));
	
	VkExtent2D actualExtent = { pVkTexture->m_width,pVkTexture->m_height };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = frameBuffer;
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = actualExtent;

	VkClearValue rtClearValue[2];
	rtClearValue[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	rtClearValue[1].depthStencil = { 0.0f,0u };

	renderPassInfo.clearValueCount = 2;
	renderPassInfo.pClearValues = rtClearValue;

	vkCmdBeginRenderPass(*m_vkCmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CVulkanContext::RHIDrawIndexedPrimitive(CRHIBuffer* indexBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
{
	if (m_gfxStateCache.m_bPipelineDirty)
	{
		vkCmdBindPipeline(*m_vkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gfxStateCache.m_pipeline);
		m_gfxStateCache.m_bPipelineDirty = false;
	}
}
