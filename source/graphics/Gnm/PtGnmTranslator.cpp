#include <assert.h>
#include "PtGnmTranslator.h"
#include "PtGPURegs.h"
#include "graphics\Gcn\PtGcnDumpShader.h"
#include "graphics\RHI\RHICommandList.h"

void CPtGnmTranslator::TranslateAndDispatchCmd(const void* commandBuffer, uint32_t commandSize)
{
	gRHICommandList.RHIBeginFrame();

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

	gRHICommandList.RHIEndFrame();
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
		onSetIndexType(pm4Hdr, (PtGfx::PM4CMDSETDATA*)itBody);
		break;
	case PtGfx::IT_SET_CONTEXT_REG:
		onSetContextRegs(pm4Hdr, (PtGfx::PM4CMDSETDATA*)itBody);
		break;
	case PtGfx::IT_SET_UCONFIG_REG:
		OnSetUConfigRegs(pm4Hdr, (PtGfx::PM4CMDSETDATA*)itBody);
		break;
	case PtGfx::IT_SET_SH_REG:
		OnSetShRegs(pm4Hdr, (PtGfx::PM4CMDSETDATA*)itBody);
		break;
	case IT_OP_CUS:
		ProcessGnmPrivateOp(pm4Hdr, itBody);
		break;
	default:
		assert(false);
		break;
	}
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
	case OP_CUS_UPDATE_VS_SHADER:
		break;
	case OP_CUS_SET_PS_SHADER:
	{
		GnmCmdSetPSShader* param = (GnmCmdSetPSShader*)pm4Hdr;
		GetGpuRegs()->SPI.PS.LO = param->psRegs.spiShaderPgmLoPs;
		GetGpuRegs()->SPI.PS.HI = param->psRegs.spiShaderPgmHiPs;
		GetGpuRegs()->SPI.PS.RSRC1 = *(PtGfx::SPI_SHADER_PGM_RSRC1_PS*)(&param->psRegs.spiShaderPgmRsrc1Ps);
		GetGpuRegs()->SPI.PS.RSRC2 = *(PtGfx::SPI_SHADER_PGM_RSRC2_PS*)(&param->psRegs.spiShaderPgmRsrc2Ps);
		GetGpuRegs()->SPI.PS.Z_FORMAT = *(PtGfx::SPI_SHADER_Z_FORMAT*)(&param->psRegs.spiShaderZFormat);
		GetGpuRegs()->SPI.PS.COL_FORMAT = *(PtGfx::SPI_SHADER_COL_FORMAT*)(&param->psRegs.spiShaderColFormat);
		GetGpuRegs()->SPI.PS.INPUT_ENA = *(PtGfx::SPI_PS_INPUT_ENA*)(&param->psRegs.spiPsInputEna);
		GetGpuRegs()->SPI.PS.INPUT_ADDR = *(PtGfx::SPI_PS_INPUT_ADDR*)(&param->psRegs.spiPsInputAddr);
		GetGpuRegs()->SPI.PS.IN_CONTROL = *(PtGfx::SPI_PS_IN_CONTROL*)(&param->psRegs.spiPsInControl);
		GetGpuRegs()->SPI.PS.BARYC_CNTL = *(PtGfx::SPI_BARYC_CNTL*)(&param->psRegs.spiBarycCntl);
		GetGpuRegs()->SPI.PS.SHADER_CONTROL = *(PtGfx::DB_SHADER_CONTROL*)(&param->psRegs.dbShaderControl);
		GetGpuRegs()->SPI.PS.SHADER_MASK = *(PtGfx::CB_SHADER_MASK*)(&param->psRegs.cbShaderMask);
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

void CPtGnmTranslator::onSetIndexType(PM4_PT_TYPE_3_HEADER* pm4Hdr, PtGfx::PM4CMDSETDATA* itBody)
{
	GetGpuRegs()->VGT_DMA.INDEX_TYPE = *(PtGfx::VGT_INDEX_TYPE_MODE*)(itBody + 1);
}

void CPtGnmTranslator::OnSetUConfigRegs(PM4_PT_TYPE_3_HEADER* pm4Hdr, PtGfx::PM4CMDSETDATA* itBody)
{
	constexpr uint32_t USERCONFIG_REG_BASE = 0x0C000;
	constexpr uint32_t USERCONFIG_REG_END = 0x10000;
	constexpr uint32_t USERCONFIG_REG_SIZE = USERCONFIG_REG_END - USERCONFIG_REG_BASE;

	uint32_t count = pm4Hdr->count;
	for (uint32_t index = 0; index < count; index++)
	{
		uint16_t reg = USERCONFIG_REG_BASE + itBody->regOffset + index;
		uint32_t value = *((uint32_t*)(itBody + 1) + index);
		SetUContextReg(reg, value);
	}
}

void CPtGnmTranslator::onSetContextRegs(PM4_PT_TYPE_3_HEADER* pm4Hdr, PtGfx::PM4CMDSETDATA* itBody)
{
	constexpr uint32_t CONTEXT_REG_BASE = 0xA000;
	constexpr uint32_t CONTEXT_REG_END = 0xA400;
	constexpr uint32_t CONTEXT_REG_SIZE = CONTEXT_REG_END - CONTEXT_REG_BASE;
	uint32_t count = pm4Hdr->count;
	for (uint32_t index = 0; index < count; index++)
	{
		uint16_t reg = CONTEXT_REG_BASE + itBody->regOffset + index;
		uint32_t value = *((uint32_t*)(itBody + 1 + index));
		SetContextReg(reg, value);
	}

	m_setCtxCount++;
}


void CPtGnmTranslator::OnSetShRegs(PM4_PT_TYPE_3_HEADER* pm4Hdr, PtGfx::PM4CMDSETDATA* itBody)
{
	constexpr uint32_t SH_REG_BASE = 0x2C00;
	constexpr uint32_t SH_REG_END = 0x3000;
	constexpr uint32_t SH_REG_SIZE = SH_REG_END - SH_REG_BASE;

	// if count == 2, the type of cmd data is pointer sotred with 2 user data slot

	uint32_t count = pm4Hdr->count;
	for (uint32_t index = 0; index < count; index++)
	{
		uint16_t reg = SH_REG_BASE + itBody->regOffset + index;
		uint32_t value = *((uint32_t*)(itBody + 1 + index));
		SetShReg(reg, value);
	}
}

void CPtGnmTranslator::SetUContextReg(uint16_t reg, uint32_t value)
{
	switch (reg)
	{
	case PtGfx::mmVGT_NUM_INSTANCES:
	{
		GetGpuRegs()->VGT_NUM_INSTANCES = *(PtGfx::VGT_NUM_INSTANCES*)(&value);
		break;
	}
	default:
		//assert(false);
		break;
	}
}

void CPtGnmTranslator::SetContextReg(uint16_t reg, uint32_t value)
{
	if (reg >= PtGfx::mmCB_COLOR0_BASE && reg <= PtGfx::mmCB_COLOR7_DCC_BASE)
	{
		((uint32_t*)GetGpuRegs()->RENDER_TARGET)[reg - PtGfx::mmCB_COLOR0_BASE] = value;
	}

	if (reg >= PtGfx::mmCB_BLEND0_CONTROL && reg <= PtGfx::mmCB_BLEND7_CONTROL)
	{
		GetGpuRegs()->CB_BLEND_CONTROL[reg - PtGfx::mmCB_BLEND0_CONTROL] = *(PtGfx::CB_BLEND0_CONTROL*)&value;
	}

	switch (reg)
	{
	case PtGfx::mmPA_SU_SC_MODE_CNTL:
		GetGpuRegs()->SC_MODE_CNTL = *(PtGfx::PA_SU_SC_MODE_CNTL*)&value;
		break;
	case PtGfx::mmDB_DEPTH_CONTROL:
		GetGpuRegs()->DEPTH.DEPTH_CONTROL = *(PtGfx::DB_DEPTH_CONTROL*)&value;
		break;
	case PtGfx::mmCB_TARGET_MASK:
		GetGpuRegs()->TARGET_MASK = *(PtGfx::CB_TARGET_MASK*)&value;
		break;
	case PtGfx::mmDB_Z_INFO:
		break;
	default:
	{
		if (reg >= PtGfx::mmCB_COLOR0_BASE && PtGfx::mmCB_COLOR7_DCC_BASE <= reg) { break; }
		if (reg >= PtGfx::mmCB_BLEND0_CONTROL && PtGfx::mmCB_BLEND7_CONTROL <= reg) { break; }
		//assert(false);
		break;
	}
	}
	
}

void CPtGnmTranslator::SetShReg(uint16_t reg, uint32_t value)
{
	if (reg >= PtGfx::mmSPI_SHADER_USER_DATA_VS_0 && reg <= PtGfx::mmSPI_SHADER_USER_DATA_VS_15)
	{
		GetGpuRegs()->SPI.VS.USER_DATA[reg - PtGfx::mmSPI_SHADER_USER_DATA_VS_0] = value;
	}
	else
	{
		assert(false);
	}
}
