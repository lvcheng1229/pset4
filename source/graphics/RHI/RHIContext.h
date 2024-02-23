#pragma once
#include "RHIResource.h"
class CRHIContext
{
public:
	virtual void RHIBeginFrame() = 0;
	virtual void RHIEndFrame() = 0;

	virtual void RHISetGraphicsPipelineState(std::shared_ptr<CRHIGraphicsPipelineState> graphicsPso) = 0;
	virtual void RHIBeginRenderPass(CRHIRenderPass* rhiRenderPass, CRHITexture2D* rtTextures, uint32_t rtNum)) = 0;
private:
};