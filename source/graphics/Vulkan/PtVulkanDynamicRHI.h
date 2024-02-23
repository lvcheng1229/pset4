#pragma once
#include "graphics\RHI\DynamicRHI.h"
#include "PtVulkanDevice.h"
class CVulkanDynamicRHI : public CDynamicRHI
{
public:
	CVulkanDynamicRHI();

	void Init(void* windowHandle) override;
	std::shared_ptr<CRHIVertexShader> RHICreateVertexShader(const std::vector<uint8_t>& code)override;
	std::shared_ptr<CRHIPixelShader> RHICreatePixelShader(const std::vector<uint8_t>& code)override;
	std::shared_ptr<CRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc)override;
private:

	VkPipelineLayout PtCreateVulkanGraphicsPipelineLayout(CRHIVertexShader* vertexShader, CRHIPixelShader* pixelShader);
	VkRenderPass PtCreateVulkanRenderPass(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc);

	CVulkanDevice m_device;
};