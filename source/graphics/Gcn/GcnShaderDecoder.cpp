#include <assert.h>
#include "GcnShaderDecoder.h"
#include "graphics\AMD\PtChip.h"

#define READ_RET_INS(type,size)\
	if ((token & SQ_ENC_##type##_MASK) == SQ_ENC_##type##_BITS)\
	{\
		const uint32_t instruction = codeReader.ReadU##size();\
		return instruction;\
	}\

#define IF_ENC_IS(ins,enc) if ((ins & SQ_ENC_##enc##_MASK) == SQ_ENC_##enc##_BITS)


const SShaderBinaryInfo* GetShaderInfo(const void* code)
{
	const uint32_t* base = reinterpret_cast<const uint32_t*>(code);
	assert(base[0] == 0xBEEB03FF);
	return reinterpret_cast<const SShaderBinaryInfo*>(base + (base[1] + 1) * 2);
}

void UpdateMaxBrach(uint16_t SIMM16, CGsCodeReader codeReader, uintptr_t& maxBranch)
{
	if (SIMM16 > 0)
	{
		uintptr_t i = codeReader.GetPos() + SIMM16 + 1;
		if (i > maxBranch)
		{
			maxBranch = i;
		}
	}
}

void CGsISAProcessor::Init(void* base)
{
	m_base = base;
	m_shaderInfo = GetShaderInfo(base);;
	parseEnd = reinterpret_cast<const uint32_t*>(base) + GetShaderInfo(base)->m_length;
}

uint32_t CGsISAProcessor::ParseSize(const void* startp, bool bStePc)
{
	//const SShaderBinaryInfo* shaderInfo = GetShaderInfo(code);
	const uint32_t* start = reinterpret_cast<const uint32_t*>(startp);
	const uint32_t* end = reinterpret_cast<const uint32_t*>(parseEnd);
	m_codeReader = CGsCodeReader(start, end);

	uintptr_t maxBranch = 0;
	while (!m_codeReader.IsEnd())
	{
		uint32_t ins = ParseInstruction(m_codeReader);
		if ((ins & SQ_ENC_SOPP_MASK) == SQ_ENC_SOPP_BITS)
		{
			PtGfx::SQ_SOPP SQ_SOPP = *(PtGfx::SQ_SOPP*)(&ins);
			switch (uint32_t(SQ_SOPP.bitfields.OP))
			{
			case SQ_S_BRANCH:
			case SQ_S_CBRANCH_SCC0:
			case SQ_S_CBRANCH_SCC1:
			case SQ_S_CBRANCH_VCCZ:
			case SQ_S_CBRANCH_VCCNZ:
			case SQ_S_CBRANCH_EXECZ:
			case SQ_S_CBRANCH_EXECNZ:
			{
				UpdateMaxBrach(SQ_SOPP.bitfields.SIMM16, m_codeReader, maxBranch);
				break;
			}
			}
		}

		if ((ins & SQ_ENC_SOPK_MASK) == SQ_ENC_SOPK_BITS)
		{
			PtGfx::SQ_SOPK SQ_SOPK = *(PtGfx::SQ_SOPK*)(&ins);
			switch (uint32_t(SQ_SOPK.bitfields.OP))
			{
			case SQ_S_CBRANCH_I_FORK:
			{
				UpdateMaxBrach(SQ_SOPK.bitfields.SIMM16, m_codeReader, maxBranch);
				break;
			}
			}
		}

		if (bStePc)
		{
			if ((ins & SQ_ENC_SOP1_MASK) == SQ_ENC_SOP1_BITS)
			{
				PtGfx::SQ_SOP1 SOP1 = *(PtGfx::SQ_SOP1*)(&ins);
				if (uint32_t(SOP1.bitfields.OP) == SQ_S_SETPC_B64)
				{
					break;
				}
			}
		}
		else
		{
			if ((ins & SQ_ENC_SOPP_MASK) == SQ_ENC_SOPP_BITS)
			{
				PtGfx::SQ_SOPP SQ_SOPP = *(PtGfx::SQ_SOPP*)(&ins);
				if (uint32_t(SQ_SOPP.bitfields.OP) == SQ_S_ENDPGM)
				{
					if (m_codeReader.GetPos() > maxBranch)
						break;
				}
			}
		}
	}
	
	uint32_t codeSize = m_codeReader.GetPos() - m_codeReader.GetBeg();
	uint32_t dis = uintptr_t(m_shaderInfo) - m_codeReader.GetPos();
	if ((dis < 256 + 7) && dis > 0)
	{
		codeSize += (dis + sizeof(SShaderBinaryInfo));
	}
	return codeSize;
}

SInputUsageSlot* CGsISAProcessor::GetShaderSlot()
{
	SInputUsageSlot* inputUsageSlot = nullptr;
	if (GetShaderInfo(m_base)->m_numInputUsageSlots > 0)
	{
		uint8_t* usageMasks = (uint8_t*)GetShaderInfo(m_base) - (GetShaderInfo(m_base)->m_chunkUsageBaseOffsetInDW * 4);
		inputUsageSlot = (SInputUsageSlot*)usageMasks - (GetShaderInfo(m_base)->m_numInputUsageSlots);
	}
	return inputUsageSlot;
}

int32_t CGsISAProcessor::GetUsageRegisterIndex(EShaderInputUsage usgae)
{
	SInputUsageSlot* usageSlot = GetShaderSlot();
	if (usageSlot != nullptr)
	{
		for (uint32_t index = 0; index < GetShaderInfo(m_base)->m_numInputUsageSlots; index++)
		{
			if (usgae == usageSlot[index].m_usageType) { return usageSlot[index].m_startRegister; };
		}
	}
	return -1;
}

uint32_t CGsISAProcessor::ParserFetchShader(const void* base)
{
	const uint32_t* start = reinterpret_cast<const uint32_t*>(base);
	const uint32_t* end = reinterpret_cast<const uint32_t*>(start + std::numeric_limits<uint32_t>::max());
	m_codeReader = CGsCodeReader(start, end);
	uint32_t semanticNum = 0;
	while (!m_codeReader.IsEnd())
	{
		//s_load_dwordx4 s[8:11], s[2:3], 0x00                      // 00000000: C0840300
		//s_load_dwordx4 s[12:15], s[2:3], 0x04                     // 00000004: C0860304
		//s_load_dwordx4 s[16:19], s[2:3], 0x08                     // 00000008: C0880308
		//s_waitcnt     lgkmcnt(0)                                  // 0000000C: BF8C007F
		//buffer_load_format_xyzw v[4:7], v0, s[8:11], 0 idxen      // 00000010: E00C2000 80020400
		//buffer_load_format_xyz v[8:10], v0, s[12:15], 0 idxen     // 00000018: E0082000 80030800
		//buffer_load_format_xy v[12:13], v0, s[16:19], 0 idxen     // 00000020: E0042000 80040C00
		//s_waitcnt     0                                           // 00000028: BF8C0000
		//s_setpc_b64   s[0:1]    

		//return the number of buffer_load_format_xxx
		uint32_t ins = ParseInstruction(m_codeReader);
		if ((ins & SQ_ENC_SOP1_MASK) == SQ_ENC_SOP1_BITS)
		{
			PtGfx::SQ_SOP1 SOP1 = *(PtGfx::SQ_SOP1*)(&ins);
			if (uint32_t(SOP1.bitfields.OP) == SQ_S_SETPC_B64)
			{
				break;
			}
		}

		if ((ins & SQ_ENC_MUBUF_MASK) == SQ_ENC_MUBUF_BITS)
		{
			semanticNum++;
		}
                                  
	}
	return semanticNum;
}

uint32_t CGsISAProcessor::ParseInstruction(CGsCodeReader& codeReader)
{
	const uint32_t token = codeReader[0];
	READ_RET_INS(SOP2, 32);
	READ_RET_INS(SOPK, 32);
	READ_RET_INS(SOP1, 32);
	READ_RET_INS(SOPC, 32);
	READ_RET_INS(SOPP, 32);
	READ_RET_INS(SMRD, 32);
	READ_RET_INS(VOP2, 32);
	READ_RET_INS(VOP1, 32);
	READ_RET_INS(VOPC, 64);
	READ_RET_INS(VOP3, 64);
	READ_RET_INS(VINTRP, 32);
	READ_RET_INS(DS, 64);
	READ_RET_INS(MUBUF, 64);
	READ_RET_INS(MTBUF, 64);
	READ_RET_INS(MIMG, 64);
	READ_RET_INS(EXP, 64);
	return 0;
}
