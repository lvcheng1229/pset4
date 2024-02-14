#include <assert.h>
#include "PtGnmTranslator.h"

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
	case IT_GNM_PRIVATE:
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
	m_gnmCmdBuffer.DrawIndex(param->indexCount, (const void*)param->indexAddr);
}

void CPtGnmTranslator::ProcessGnmPrivateOp(PM4_PT_TYPE_3_HEADER* pm4Hdr, uint32_t* itBody)
{
	uint8_t opCodePri = PM4_PRIV(*(uint32_t*)pm4Hdr);
	switch (opCodePri)
	{
	case OP_PRIV_INITIALIZE_DEFAULT_HARDWARE_STATE:
	case OP_PRIV_WAIT_UNTIL_SAFE_FOR_RENDERING:
		//TODO:
		break;
	case OP_PRIV_SET_VS_SHADER:
	{
		GnmCmdVSShader* param = (GnmCmdVSShader*)pm4Hdr;
		m_gnmCmdBuffer.SetVsShader(&param->vsRegs, param->modifier);
		break;
	}
		
	case OP_PRIV_SET_PS_SHADER:
	{
		const GnmCmdPSShader* param = (GnmCmdPSShader*)pm4Hdr;
		m_gnmCmdBuffer.SetPsShader(&param->psRegs);
		break;
	}

	case OP_PRIV_DRAW_INDEX:
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