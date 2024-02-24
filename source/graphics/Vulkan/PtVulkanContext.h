#pragma once
#include <memory>

#include "vulkan\vulkan.h"

#include "core\PtUtil.h"

#include "PtVulkanResource.h"
#include "graphics\RHI\RHIContext.h"


class CVulkanDevice;

struct SVertexBufferStreaming
{
	CRHIBuffer* m_buffer;
	uint32_t offset;
};

struct SVkGraphicsStateCache
{
	VkPipeline m_pipeline;
	bool m_bPipelineDirty;

	SVertexBufferStreaming m_vertexStreamings[8];
	bool m_bVertexBufferDirty[8] = { 0,0,0,0,0,0,0,0 };
};

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
	virtual void RHIBeginRenderPass(CRHIRenderPass* rhiRenderPass, CRHITexture2D* rtTextures, uint32_t rtNum, CRHITexture2D* dsTexture)override;

	virtual void RHISetVertexBuffer(CRHIBuffer* vtxBuffer, uint32_t bufferSlot, uint32_t bufferOffset)override;
	virtual void RHIDrawIndexedPrimitive(CRHIBuffer* indexBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)override;
private:
	SVkGraphicsStateCache m_gfxStateCache;

	// viewport
	VkFramebuffer PtGetOrCreateVulkanFrameBuffer(const CVulkanTexture2D* pVkRtTexture2D, VkRenderPass renderPass, const CVulkanTexture2D* pVkDsTexture2D);

	VkCommandBuffer* m_vkCmdBuffer;
	CVulkanDevice* m_device;
};