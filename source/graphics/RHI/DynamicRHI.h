#pragma once
#include <vector>
#include <memory>
#include "RHIResource.h"

class CDynamicRHI
{
public:
	virtual void Init(void* windowHandle) = 0;
	virtual std::shared_ptr<CRHITexture2D> RHIGetDeviceDefaultTexture(EDeviceDefaultTex deviceDefaultTex) = 0;
	virtual std::shared_ptr<CRHIVertexShader> RHICreateVertexShader(const std::vector<uint8_t>& code) = 0;
	virtual std::shared_ptr<CRHIPixelShader> RHICreatePixelShader(const std::vector<uint8_t>& code) = 0;
	virtual std::shared_ptr<CRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const CRHIGraphicsPipelineStateInitDesc& psoInitDesc) = 0;
	virtual std::shared_ptr<CRHIRenderPass> RHICreateRenderPass(const CRHIRenderPassInfo& renderPass) = 0;
	virtual std::shared_ptr<CRHIBuffer> RHICreateBuffer(const void* pInitData, uint64_t nElementCount, uint64_t nStride, EBufferUsage bufferUsage) = 0;
	virtual std::shared_ptr<CRHITexture2D> RHICreateTexture2D(const void* pInitData, uint32_t width, uint32_t height, PtGfx::IMG_DATA_FORMAT dataFormat, PtGfx::IMG_NUM_FORMAT numFormat, ETextureCreateFlags texFlag) = 0;
	virtual void RHIUpdateBuffer(CRHIBuffer* pBuffer, uint8_t* pData, uint64_t bufferSize) = 0;
private:
};

extern CDynamicRHI* gPlatformRHI;
void InitDynamicRHI(void* windowHandle, bool useVulkan = true, bool useDx12 = false);

inline std::shared_ptr<CRHITexture2D> RHIGetDeviceDefaultTexture(EDeviceDefaultTex deviceDefaultTex)
{
	return gPlatformRHI->RHIGetDeviceDefaultTexture(deviceDefaultTex);
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

inline std::shared_ptr<CRHITexture2D> RHICreateTexture2D(const void* pInitData, uint32_t width, uint32_t height, PtGfx::IMG_DATA_FORMAT dataFormat, PtGfx::IMG_NUM_FORMAT numFormat, ETextureCreateFlags texFlag)
{
	return gPlatformRHI->RHICreateTexture2D(pInitData, width, height, dataFormat, numFormat, texFlag);
}

inline void RHIUpdateBuffer(CRHIBuffer* pBuffer, uint8_t* pData, uint64_t bufferSize)
{
	gPlatformRHI->RHIUpdateBuffer(pBuffer, pData, bufferSize);
}

inline std::shared_ptr<CRHIBuffer> RHICreateBuffer(const void* pInitData, uint64_t nTotalSize, uint64_t nStride, EBufferUsage bufferUsage)
{
	return gPlatformRHI->RHICreateBuffer(pInitData, nTotalSize, nStride, bufferUsage);
}





