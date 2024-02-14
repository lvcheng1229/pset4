#pragma once
#include <array>
#include <unordered_map>

#include "graphics\Gcn\PtGcnShader.h"
#include "graphics\Gnm\PtGnmShader.h"

#include "GnmStructure.h"

struct SGnmShaderContext
{
	const void* code = nullptr;
	std::array<uint32_t, GnmMaxUserDataCount>m_resourceSlot;
	SGcnShaderMeta meta;
};

struct SGnmGraphicsState
{
	std::array<SGnmShaderContext, eShaderStateCount> m_shaderStages = {};
};

struct SGnmRenderState
{
	SGnmGraphicsState m_graphicsState;
};

class CPtGnmCommandBuffer
{
public:
	void SetVsShader(VsStageRegisters* vsRegs, uint32_t shaderModifier);
	void SetPsShader(const PsStageRegisters* psRegs);
	void DrawIndex(uint32_t indexCount, const void* indexAddr);
private:
	void GenerateIndexBuffer(uint32_t indexCount, const void* indexAddr);

	void CommitGraphicsState();
	void UpdateVertexShaderStageInternal();

	CGnmShader GetOrAddShader(const void* code);

	SGnmRenderState m_gnmRenderState;
	std::unordered_map<std::size_t, CGnmShader> m_shaderCached;
};