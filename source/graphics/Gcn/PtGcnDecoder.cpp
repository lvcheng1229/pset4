#include <assert.h>
#include "core\Log.h"
#include "PtGcnDecoder.h"

void CGcnDecodeContext::DecodeInstruction(CGcnCodeReader& code)
{
	const uint32_t token = code[0];
	uint32_t instructionEncoding = GetInstructionEncoding(token);
	if (instructionEncoding == INS_ENC_INVALID)
	{
		PSET_LOG_ERROR("invalid instruction encoding");
	}
	uint32_t encodingLength = GetEncodingLength(instructionEncoding);
	m_gcnInstruction = SGcnShaderInstruction();

	if (encodingLength == sizeof(uint32_t))
	{

	}
	else
	{

	}
}

#define CMP_RET_INS_ENC(INSENC)\
if((token & PtGfx::XGfx::SQ_ENC_##INSENC##_MASK) == PtGfx::XGfx::SQ_ENC_##INSENC##_BITS)\
{\
	return PtGfx::XGfx::SQ_ENC_##INSENC##_BITS;\
}\

uint32_t CGcnDecodeContext::GetInstructionEncoding(uint32_t token)
{
	CMP_RET_INS_ENC(SOP1);
	CMP_RET_INS_ENC(SOPP);
	CMP_RET_INS_ENC(SOPC);

	CMP_RET_INS_ENC(VOP1);
	CMP_RET_INS_ENC(VOPC);

	CMP_RET_INS_ENC(VOP3);
	CMP_RET_INS_ENC(EXP);
	CMP_RET_INS_ENC(VINTRP);
	CMP_RET_INS_ENC(DS);
	CMP_RET_INS_ENC(MUBUF);
	CMP_RET_INS_ENC(MTBUF);
	CMP_RET_INS_ENC(MIMG);

	assert(false);
	return INS_ENC_INVALID;
}

uint32_t CGcnDecodeContext::GetEncodingLength(uint32_t instructionEnc)
{
	switch (instructionEnc)
	{
	case PtGfx::XGfx::SQ_ENC_SOP1_BITS:
	case PtGfx::XGfx::SQ_ENC_SOPP_BITS:
	case PtGfx::XGfx::SQ_ENC_SOPC_BITS:
	case PtGfx::XGfx::SQ_ENC_VOP1_BITS:
	case PtGfx::XGfx::SQ_ENC_VOPC_BITS:
	case PtGfx::XGfx::SQ_ENC_VINTRP_BITS:
		return sizeof(uint32_t);
	case PtGfx::XGfx::SQ_ENC_VOP3_BITS:
	case PtGfx::XGfx::SQ_ENC_MUBUF_BITS:
	case PtGfx::XGfx::SQ_ENC_MTBUF_BITS:
	case PtGfx::XGfx::SQ_ENC_MIMG_BITS:
	case PtGfx::XGfx::SQ_ENC_DS_BITS:
	case PtGfx::XGfx::SQ_ENC_EXP_BITS:
		return sizeof(uint64_t);
	default:
		assert(false);
		return 0;
	}
	return 0;
}
