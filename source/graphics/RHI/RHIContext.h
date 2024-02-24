#pragma once

#include <memory>
#include "RHIResource.h"

class CRHIContext
{
public:
	virtual void RHIBeginFrame() = 0;
	virtual void RHIEndFrame() = 0;

	virtual void RHISetVertexBuffer(CRHIBuffer* vtxBuffer, uint32_t bufferSlot, uint32_t bufferOffset) = 0;
	virtual void RHISetGraphicsPipelineState(std::shared_ptr<CRHIGraphicsPipelineState> graphicsPso) = 0;
	virtual void RHIBeginRenderPass(CRHIRenderPass* rhiRenderPass, CRHITexture2D* rtTextures, uint32_t rtNum, CRHITexture2D* dsTexture) = 0;
	virtual void RHIDrawIndexedPrimitive(CRHIBuffer* indexBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance) = 0;
private:
};