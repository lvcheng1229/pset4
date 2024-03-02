#pragma once
#include <stdint.h>
#include <vector>
#include <memory>

#include "graphics\AMD\pal\core\pal.h"
#include "graphics\Gnm\PtGPURegs.h"

#include "RHI.h"

enum EBufferUsage : uint32_t
{
	RHIBU_VB = (1 << 0), // vertex buffer
	RHIBU_IB = (1 << 1), // index buffer
	RHIBU_CB = (1 << 2), // constant buffer
};

enum class EIndexType : uint32_t
{
	IDX_16,
	IDX_32,
};

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

class CRHIBuffer
{
public:
	uint32_t m_elemCount;
	uint32_t m_elemStride;
};

static constexpr uint32_t gMaxResourceNum = 8;

class CRHIShader
{
public:
	uint32_t m_numPushConst;
	uint32_t m_pushCtStartRegIdnex;
	std::vector<size_t> m_pushConstSizes;
	
	uint32_t m_numCbv;
	uint32_t m_cbvStartRegIdnex;
	std::shared_ptr<CRHIBuffer> m_pConstantBuffers[gMaxResourceNum];

	uint32_t m_numSrv;
	uint32_t m_srvStartIndex;
	//std::shared_ptr<CRHITexture2D> m_pTextures[gMaxResourceNum];

	uint32_t m_numUav;
	uint32_t m_uavStartIndex;

	uint32_t m_numSampler;
	uint32_t m_samplerStartIndex;

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
	PtGfx::CB_COLOR_CONTROL m_pipelineColorBlendControl; // todo: remove this member

	SRenderTarget renderTargets[Pal::MaxColorTargets];
	
	// depth stencil state
	CRHIDepthStencilState m_dsState;
	CRHIRenderPass* m_rhiRenderPass;
private:

};

class CRHIGraphicsPipelineState
{
public:
	CRHIVertexShader* m_pVertexShader;
	CRHIPixelShader* m_pPixelShader;
};


class CRHIRenderPassInfo
{
public:
	// vkRenderPass
	uint32_t m_rtNum;
	CRHIDepthStencilState m_dsState;
	SRenderTarget renderTargets[Pal::MaxColorTargets];
};

