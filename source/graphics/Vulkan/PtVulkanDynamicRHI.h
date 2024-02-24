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
	
private:

	VkPipelineLayout PtCreateVulkanGraphicsPipelineLayout(CRHIVertexShader* vertexShader, CRHIPixelShader* pixelShader);
	

	CVulkanDevice m_device;
};