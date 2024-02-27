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

struct SPixelShaderPushConstant
{
	std::vector<uint8_t> m_pushConstantData;
};

struct SBufferDesc
{
	VkBuffer m_buffer;
	uint32_t m_size;
};

struct STextureAndSamplerDesc
{
	VkImageView m_imgView;
	VkSampler m_vkSampler;
};

static constexpr uint32_t gMaxVkResourceNum = 4;

struct SVkGraphicsStateCache
{
	CRHIVertexShader* m_pVertexShader;
	CRHIPixelShader* m_pPixelShader;

	SPixelShaderPushConstant m_pixelShaderPushConstant[4];
	bool m_bPushCtDirty[4] = { 0,0,0,0 };

	SBufferDesc m_bufferDescs[gMaxVkResourceNum];
	STextureAndSamplerDesc m_texAndSamplerDescs[gMaxVkResourceNum];

	VkPipeline m_pipeline;
	VkPipelineLayout m_pipelineLayout;
	bool m_bPipelineDirty;

	SVertexBufferStreaming m_vertexStreamings[8];
	bool m_bVertexBufferDirty[8] = { 0,0,0,0,0,0,0,0 };

	bool m_bInRenderPass = false;
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
	virtual void RHIEndRenderPass() override;

	virtual void RHIPixelShaderSetPushConstatnt(uint32_t index, uint32_t size, uint8_t* pData)override;

	virtual void RHISetConstantBuffer(CRHIBuffer* ctBuffer, uint32_t index)override;
	virtual void RHISetVertexBuffer(CRHIBuffer* vtxBuffer, uint32_t bufferSlot, uint32_t bufferOffset)override;
	
	virtual void RHIDrawIndexedPrimitive(CRHIBuffer* indexBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)override;
private:
	SVkGraphicsStateCache m_gfxStateCache;

	// viewport
	VkFramebuffer PtGetOrCreateVulkanFrameBuffer(const CVulkanTexture2D* pVkRtTexture2D, VkRenderPass renderPass, const CVulkanTexture2D* pVkDsTexture2D);

	VkCommandBuffer* m_vkCmdBuffer;
	CVulkanDevice* m_device;
};