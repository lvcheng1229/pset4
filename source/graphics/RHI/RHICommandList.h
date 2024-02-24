#pragma once
#include "DynamicRHI.h"
#include "RHIContext.h"

class CRHICommnadList
{
public:
	inline void RHIBeginFrame();
	inline void RHIEndFrame();
	inline void SetRHIContext(CRHIContext* gfxCtx) { m_gfxCtx = gfxCtx; }
	inline void RHISetGraphicsPipelineState(std::shared_ptr<CRHIGraphicsPipelineState> graphicsPso);
	
	inline void RHISetVertexBuffer(CRHIBuffer* vtxBuffer, uint32_t bufferSlot, uint32_t bufferOffset);


	inline void RHIBeginRenderPass(CRHIRenderPass* rhiRenderPass, CRHITexture2D* rtTextures, uint32_t rtNum, CRHITexture2D* dsTexture);
	inline void RHIDrawIndexedPrimitive(CRHIBuffer* indexBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance);
private:
	CRHIContext* m_gfxCtx;
};

extern CRHICommnadList gRHICommandList;

inline void CRHICommnadList::RHIBeginFrame()
{
	m_gfxCtx->RHIBeginFrame();
}

inline void CRHICommnadList::RHIEndFrame()
{
	m_gfxCtx->RHIEndFrame();
}

inline void CRHICommnadList::RHISetGraphicsPipelineState(std::shared_ptr<CRHIGraphicsPipelineState> graphicsPso)
{
	m_gfxCtx->RHISetGraphicsPipelineState(graphicsPso);
}

inline void CRHICommnadList::RHISetVertexBuffer(CRHIBuffer* vtxBuffer, uint32_t bufferSlot, uint32_t bufferOffset)
{
	m_gfxCtx->RHISetVertexBuffer(vtxBuffer, bufferSlot, bufferOffset);
}

void CRHICommnadList::RHIBeginRenderPass(CRHIRenderPass* rhiRenderPass, CRHITexture2D* rtTextures, uint32_t rtNum, CRHITexture2D* dsTexture)
{
	m_gfxCtx->RHIBeginRenderPass(rhiRenderPass, rtTextures, rtNum, dsTexture);
}

inline void CRHICommnadList::RHIDrawIndexedPrimitive(CRHIBuffer* indexBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
{
	m_gfxCtx->RHIDrawIndexedPrimitive(indexBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

