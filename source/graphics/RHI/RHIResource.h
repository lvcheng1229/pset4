#pragma once
#include <stdint.h>
#include <vector>

#include "graphics\AMD\pal\core\pal.h"
#include "graphics\Gnm\PtGPURegs.h"

#include "RHI.h"

enum ERHIShaderType :uint8_t
{
	RST_VertexShader,
	RST_PixelShader,
	RST_ShaderCount,
};

enum class EDeviceDefaultTex
{
	DDT_BackBuffer,
	DDT_DepthStencil,
};

enum ETextureCreateFlags
{
	TexCreate_None = 0,
	TexCreate_RenderTargetable = 1 << 0,
	TexCreate_DepthStencilTargetable = 1 << 1,
	TexCreate_ShaderResource = 1 << 2,
	TexCreate_SRGB = 1 << 3,
	TexCreate_UAV = 1 << 4,
};

class CRHITexture2D
{
public:
	uint32_t m_width;
	uint32_t m_height;
};

class CRHIShader
{
public:
	uint32_t m_numCbv;
	uint32_t m_numSrv;
	uint32_t m_numUav;
	uint32_t m_numSampler;
};

class CRHIVertexShader : public CRHIShader
{
public:
};


class CRHIPixelShader : public CRHIShader
{
public:
};

class CRHIDepthStencilState
{
public:
	bool bDepthWriteEnable;
	bool bDepthTestEnable;
	uint8_t zFunc;
};

class CRHIBuffer
{

};

class CRHIRenderPass
{

};

class CRHIGraphicsPipelineStateInitDesc
{
public:
	// vertex shader
	CRHIVertexShader* m_pVertexShader;
	CRHIPixelShader* m_pPixelShader;

	// vertex layout
	std::vector<SVertexElement> m_vertexElements;
	std::vector<SVertexBinding>m_vertexBindings;

	// render target format
	uint32_t m_rtNum;
	PtGfx::CB_BLEND0_CONTROL m_colorBelndControls[Pal::MaxColorTargets];
	SRenderTarget renderTargets[Pal::MaxColorTargets];
	
	// depth stencil state
	CRHIDepthStencilState m_dsState;
	CRHIRenderPass* m_rhiRenderPass;
private:

};

class CRHIGraphicsPipelineState
{

};


class CRHIRenderPassInfo
{
public:
	// vkRenderPass
	uint32_t m_rtNum;
	CRHIDepthStencilState m_dsState;
	SRenderTarget renderTargets[Pal::MaxColorTargets];
};

