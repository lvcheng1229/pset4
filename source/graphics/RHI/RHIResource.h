#pragma once
#include <stdint.h>
#include <vector>
#include "RHI.h"

enum ERHIShaderType :uint8_t
{
	RST_VertexShader,
	RST_PixelShader,
	RST_ShaderCount,
};

class CRHIVertexShader
{
public:
};


class CRHIPixelShader
{
public:
};

class CRHIGraphicsPipelineStateInitDesc
{
public:
	CRHIVertexShader* m_pVertexShader;
	CRHIPixelShader* m_pPixelShader;
	std::vector<SVertexElement> m_vertexElements;
	std::vector<SVertexBinding>m_vertexBindings;
private:
};

class CRHIGraphicsPipelineState
{

};