#include "graphics\AMD\gfx9\PtChip.h"
#include "graphics\Gcn\PtGcnShaderInfo.h"
#include "PtGnmCommandBuffer.h"

void CPtGnmCommandBuffer::SetVsShader(VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	SGnmShaderContext& gnmShaderCtx = m_gnmRenderState.m_graphicsState.m_shaderStages[eShaderStageVS];
	gnmShaderCtx.code = vsRegs->getCodeAddress();
	const SPI_SHADER_PGM_RSRC2_VS* rsrc2 = reinterpret_cast<const SPI_SHADER_PGM_RSRC2_VS*>(&vsRegs->spiShaderPgmRsrc2Vs);
	gnmShaderCtx.meta.vs.userSgprCount = rsrc2->bitfields.USER_SGPR;
}

void CPtGnmCommandBuffer::SetPsShader(const PsStageRegisters* psRegs)
{
	SGnmShaderContext& gnmShaderCtx = m_gnmRenderState.m_graphicsState.m_shaderStages[eShaderStagePS];
	gnmShaderCtx.code = psRegs->getCodeAddress();

	const SPI_SHADER_PGM_RSRC2_PS* rsrc2 = reinterpret_cast<const SPI_SHADER_PGM_RSRC2_PS*>(&psRegs->spiShaderPgmRsrc2Ps);
	const SPI_PS_INPUT_ENA* addr = reinterpret_cast<const SPI_PS_INPUT_ENA*>(&psRegs->spiPsInputAddr);

	gnmShaderCtx.meta.ps.userSgprCount = rsrc2->bitfields.USER_SGPR;
	gnmShaderCtx.meta.ps.m_psInputEna = *addr;
}

void CPtGnmCommandBuffer::DrawIndex(uint32_t indexCount, const void* indexAddr)
{
	GenerateIndexBuffer(0, nullptr);
}

void CPtGnmCommandBuffer::GenerateIndexBuffer(uint32_t indexCount, const void* indexAddr)
{
	//TODO:
	//uint32_t indexBufferSize = :sizeof(uint32_t) * indexCount;

	CommitGraphicsState();
}

void CPtGnmCommandBuffer::CommitGraphicsState()
{
	UpdateVertexShaderStageInternal();
}

void CPtGnmCommandBuffer::UpdateVertexShaderStageInternal()
{
	SGnmShaderContext& gnmShaderCtx = m_gnmRenderState.m_graphicsState.m_shaderStages[eShaderStageVS];
	CGnmShader gnmShader = GetOrAddShader(gnmShaderCtx.code);
	gnmShader.Compile();
}

CGnmShader CPtGnmCommandBuffer::GetOrAddShader(const void* code)
{
	SGcnBinaryInfo gcnBinInfo(code);
	std::size_t hashKey = gcnBinInfo.GetCombinedHash();

	//TODO:LOCK
	auto iter = m_shaderCached.find(hashKey);
	if (iter != m_shaderCached.end())
	{
		return iter->second;
	}

	CGnmShader shader = CGnmShader(hashKey, code);
	m_shaderCached.insert({ hashKey ,shader });
	return shader;
}
