#include <assert.h>
#include "PtGnmTranslator.h"
#include "graphics\AMD\PtGPURegs.h"

void CPtGnmTranslator::TranslateAndDispatchCmd(const void* commandBuffer, uint32_t commandSize)
{
	const PM4_HEADER_COMMON* pm4Hdr = reinterpret_cast<const PM4_HEADER_COMMON*>(commandBuffer);
	uint32_t processedCmdBufferLen = 0;
	while (processedCmdBufferLen < commandSize)
	{
		uint32_t pm4Type = pm4Hdr->type;
		switch (pm4Type)
		{
		case PM4_TYPE_0:
		case PM4_TYPE_2:
			assert(false);
			break;
		case PM4_TYPE_3:
			ProcessPM4Type3((PM4_PT_TYPE_3_HEADER*)pm4Hdr, (uint32_t*)(pm4Hdr + 1));
			break;
		}

		if (m_flipPacketDone)
		{
			m_flipPacketDone = false;
			break;
		}

		uint32_t processedPm4Count = 1;
		if (m_skipPm4Count != 0)
		{
			processedPm4Count += m_skipPm4Count;
			m_skipPm4Count = 0;
		}

		const PM4_HEADER_COMMON* nextPm4Hdr = GetNextPm4(pm4Hdr, processedPm4Count);
		uint32_t          processedLength = reinterpret_cast<uintptr_t>(nextPm4Hdr) - reinterpret_cast<uintptr_t>(pm4Hdr);
		pm4Hdr = nextPm4Hdr;
		processedCmdBufferLen += processedLength;
	}
}

void CPtGnmTranslator::ProcessPM4Type3(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody)
{
	uint32_t opcode = pm4Hdr->opcode;
	switch (opcode)
	{
	case PtGfx::IT_NOP:
	case PtGfx::IT_ACQUIRE_MEM:
	case PtGfx::IT_DMA_DATA:
	case PtGfx::IT_INDEX_TYPE:
	case PtGfx::IT_SET_CONTEXT_REG:
	case PtGfx::IT_SET_UCONFIG_REG:
		//TODO
		break;
	case PtGfx::IT_SET_SH_REG:
		SetShReg(pm4Hdr, itBody);
		break;
	case IT_OP_CUS:
		ProcessGnmPrivateOp(pm4Hdr, itBody);
		break;
	default:
		assert(false);
		break;
	}
}

void CPtGnmTranslator::ProcessGnmPrivateOpDrawIndex(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody)
{
	GnmCmdDrawIndex* param = (GnmCmdDrawIndex*)pm4Hdr;
}

void CPtGnmTranslator::ProcessGnmPrivateOp(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody)
{
	uint8_t opCodePri = PM4_PRIV(*(uint32_t*)pm4Hdr);
	switch (opCodePri)
	{
	case OP_CUS_INITIALIZE_DEFAULT_HARDWARE_STATE:
	case OP_CUS_WAIT_UNTIL_SAFE_FOR_RENDERING:
		//TODO:
		break;
	case OP_CUS_SET_VS_SHADER:
	{
		GnmCmdSetVSShader* param = (GnmCmdSetVSShader*)pm4Hdr;
		GetGpuRegs()->SPI.VS.LO = param->vsRegs.spiShaderPgmLoVs;
		GetGpuRegs()->SPI.VS.HI = param->vsRegs.spiShaderPgmHiVs;
		GetGpuRegs()->SPI.VS.RSRC1 = *(PtGfx::SPI_SHADER_PGM_RSRC1_VS*)(&param->vsRegs.spiShaderPgmRsrc1Vs);
		GetGpuRegs()->SPI.VS.RSRC2 = *(PtGfx::SPI_SHADER_PGM_RSRC2_VS*)(&param->vsRegs.spiShaderPgmRsrc2Vs);
		GetGpuRegs()->SPI.VS.OUT_CONFIG = *(PtGfx::SPI_VS_OUT_CONFIG*)(&param->vsRegs.spiVsOutConfig);
		GetGpuRegs()->SPI.VS.POS_FORMAT = *(PtGfx::SPI_SHADER_POS_FORMAT*)(&param->vsRegs.spiShaderPosFormat);
		GetGpuRegs()->SPI.VS.OUT_CNTL = *(PtGfx::PA_CL_VS_OUT_CNTL*)(&param->vsRegs.paClVsOutCntl);
		break;
	}
		
	case OP_CUS_SET_PS_SHADER:
	{
		const GnmCmdSetPSShader* param = (GnmCmdSetPSShader*)pm4Hdr;
		break;
	}

	case OP_CUS_DRAW_INDEX:
	{
		ProcessGnmPrivateOpDrawIndex(pm4Hdr, itBody);
		break;
	}
	default :
		assert(false);
		break;
	}
}

const PM4_HEADER_COMMON* CPtGnmTranslator::GetNextPm4(const PM4_HEADER_COMMON* currentPm4, uint32_t step)
{
	const PM4_HEADER_COMMON* currPm4 = currentPm4;
	while (step)
	{
		currPm4 = reinterpret_cast<const PM4_HEADER_COMMON*>((uint32_t*)currPm4 + PM4_LENGTH_DW(currPm4->u32All));
		step--;
	}
	return currPm4;
}

void CPtGnmTranslator::SetShReg(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody)
{
	constexpr uint32_t SH_REG_BASE = 0x2C00;
	constexpr uint32_t SH_REG_END = 0x3000;
	constexpr uint32_t SH_REG_SIZE = SH_REG_END - SH_REG_BASE;

	uint32_t count = pm4Hdr->count;
	for (uint32_t index = 0; index < count; index++)
	{
		//uint8_t reg =  SH_REG_BASE + Body ^ .REG_OFFSET + i;
	}
}
