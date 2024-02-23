#pragma once
#include <memory>

#include "vulkan\vulkan.h"
#include "PtVulkanDevice.h"

#include "graphics\RHI\RHIContext.h"

class CVulkanContext : public CRHIContext
{
public:
	CVulkanContext(CVulkanDevice* inVkDevice, VkCommandBuffer* cmdBuffer)
		:m_device(inVkDevice),
		 m_vkCmdBuffer(cmdBuffer)
	{};

	virtual void RHIBeginFrame()override;
	virtual void RHIEndFrame()override;

	virtual void RHISetGraphicsPipelineState(std::shared_ptr<CRHIGraphicsPipelineState> graphicsPso)override;
	virtual void RHIBeginRenderPass(CRHIRenderPass* rhiRenderPass, CRHITexture2D* rtTextures, uint32_t rtNum))override;
private:

	// viewport
	VkFramebuffer PtGetOrCreateVulkanFrameBuffer(const CRHIRenderPassInfo& renderPassInfo, VkRenderPass renderPass);

	VkCommandBuffer* m_vkCmdBuffer;
	CVulkanDevice* m_device;
};