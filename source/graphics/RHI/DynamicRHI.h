#pragma once
#include <vector>
#include <memory>
#include "RHIResource.h"

class CDynamicRHI
{
public:
	virtual void Init(void* windowHandle) = 0;
	virtual std::shared_ptr<CRHITexture2D> RHIGetBackBufferTexture() = 0;
	virtual std::shared_ptr<CRHIVertexShader> RHICreateVertexShader(const std::vector<uint8_t>& code) = 0;
	virtual std::shared_ptr<CRHIPixelShader> RHICreatePixelShader(const std::vector<uint8_t>& code) = 0;
	virtual std::shared_ptr<CRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc) = 0;
	virtual std::shared_ptr<CRHIRenderPass> RHICreateRenderPass(const CRHIRenderPassInfo& renderPass) = 0;
private:
};

extern CDynamicRHI* gPlatformRHI;
void InitDynamicRHI(void* windowHandle, bool useVulkan = true, bool useDx12 = false);

inline std::shared_ptr<CRHITexture2D> RHIGetBackBufferTexture()
{
	return gPlatformRHI->RHIGetBackBufferTexture();
}

inline std::shared_ptr<CRHIVertexShader> RHICreateVertexShader(const std::vector<uint8_t>& code)
{
	return gPlatformRHI->RHICreateVertexShader(code);
}

inline std::shared_ptr<CRHIPixelShader> RHICreatePixelShader(const std::vector<uint8_t>& code)
{
	return gPlatformRHI->RHICreatePixelShader(code);
}

inline std::shared_ptr<CRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc)
{
	return gPlatformRHI->RHICreateGraphicsPipelineState(psoInitDesc);
}

inline std::shared_ptr<CRHIRenderPass> RHICreateRenderPass(const CRHIRenderPassInfo& renderPass)
{
	return gPlatformRHI->RHICreateRenderPass(renderPass);
}



