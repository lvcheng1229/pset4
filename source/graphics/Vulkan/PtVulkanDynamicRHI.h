#pragma once
#include "graphics\RHI\DynamicRHI.h"
#include "PtVulkanDevice.h"
class CVulkanDynamicRHI : public CDynamicRHI
{
public:
	CVulkanDynamicRHI();

	void Init(void* windowHandle) override;
	std::shared_ptr<CRHITexture2D> RHIGetDeviceDefaultTexture(EDeviceDefaultTex deviceDefaultTex)override;
	std::shared_ptr<CRHIVertexShader> RHICreateVertexShader(const std::vector<uint8_t>& code)override;
	std::shared_ptr<CRHIPixelShader> RHICreatePixelShader(const std::vector<uint8_t>& code)override;
	std::shared_ptr<CRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc)override;
	std::shared_ptr<CRHIRenderPass> RHICreateRenderPass(const CRHIRenderPassInfo& renderPass)override;
	std::shared_ptr<CRHITexture2D> RHICreateTexture2D(const void* pInitData, uint32_t width, uint32_t height, PtGfx::IMG_DATA_FORMAT dataFormat, PtGfx::IMG_NUM_FORMAT numFormat, ETextureCreateFlags texFlag)override;
	std::shared_ptr<CRHIBuffer> RHICreateBuffer(const void* pInitData, uint64_t nElementCount, uint64_t nStride, EBufferUsage bufferUsage)override;
	void RHIUpdateBuffer(CRHIBuffer* pBuffer, uint8_t* pData, uint64_t bufferSize)override;
private:
	//temporary code
	void PtTransistionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	VkCommandBuffer PtBeginImmediateCommandBuffer();
	void PtEndImmediateCommandBuffer(VkCommandBuffer vkCmdBuffer);
	VkPipelineLayout PtCreateVulkanGraphicsPipelineLayout(CRHIVertexShader* vertexShader, CRHIPixelShader* pixelShader, SVulkanPipelineDescSet* outPipelineDescSet);
	

	CVulkanDevice m_device;
};