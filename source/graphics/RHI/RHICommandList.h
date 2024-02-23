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
	inline void RHIBeginRenderPass(const CRHIRenderPassInfo& renderPassInfo);
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

void CRHICommnadList::RHIBeginRenderPass(const CRHIRenderPassInfo& renderPassInfo)
{
	m_gfxCtx->RHIBeginRenderPass(renderPassInfo);
}

