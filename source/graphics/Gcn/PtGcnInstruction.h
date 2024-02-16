#pragma once
#include "graphics\AMD\gfx9\PtChip.h"

#define INS_ENC_INVALID 0x1u

enum GcnEncodingMask : uint32_t
{
	GCN_MASK_9BIT = 0x000001FFULL << 23,
	GCN_MASK_7BIT = 0x0000007FULL << 25,
	GCN_MASK_6BIT = 0x0000003FULL << 26,
	GCN_MASK_5BIT = 0x0000001FULL << 27,
	GCN_MASK_4BIT = 0x0000000FULL << 28,
	GCN_MASK_2BIT = 0x00000003ULL << 30,
	GCN_MASK_1BIT = 0x00000001ULL << 31
};

enum class GcnInstClass : uint32_t
{
	Undefined,

	ScalarArith,
	ScalarAbs,
	ScalarMov,
	ScalarMovRel,
	ScalarCmp,
	ScalarSelect,
	ScalarBitLogic,
	ScalarBitManip,
	ScalarBitField,
	ScalarConv,
	ScalarExecMask,
	ScalarQuadMask,

	VectorRegMov,
	VectorMovRel,
	VectorLane,
	VectorBitLogic,
	VectorBitField32,
	VectorThreadMask,
	VectorBitField64,
	VectorFpArith32,
	VectorFpRound32,
	VectorFpField32,
	VectorFpTran32,
	VectorFpCmp32,
	VectorFpArith64,
	VectorFpRound64,
	VectorFpField64,
	VectorFpTran64,
	VectorFpCmp64,
	VectorIntArith32,
	VectorIntArith64,
	VectorIntCmp32,
	VectorIntCmp64,
	VectorConv,
	VectorFpGraph32,
	VectorIntGraph,
	VectorMisc,

	ScalarProgFlow,
	ScalarSync,
	ScalarWait,
	ScalarCache,
	ScalarPrior,
	ScalarRegAccess,
	ScalarMsg,

	ScalarMemRd,
	ScalarMemUt,

	VectorMemBufNoFmt,
	VectorMemBufFmt,
	VectorMemBufAtomic,
	VectorMemImgNoSmp,
	VectorMemImgSmp,
	VectorMemImgUt,
	VectorMemL1Cache,

	DsIdxRd,
	DsIdxWr,
	DsIdxWrXchg,
	DsIdxCondXchg,
	DsIdxWrap,
	DsAtomicArith32,
	DsAtomicArith64,
	DsAtomicMinMax32,
	DsAtomicMinMax64,
	DsAtomicCmpSt32,
	DsAtomicCmpSt64,
	DsAtomicLogic32,
	DsAtomicLogic64,
	DsAppendCon,
	DsDataShareUt,
	DsDataShareMisc,
	GdsSync,
	GdsOrdCnt,

	VectorInterpFpCache,

	Exp,

	DbgProf
};

enum class GcnInstCategory : uint32_t
{
	Undefined,
	/// Scalar ALU Operation
	ScalarALU,
	/// Scalar Instruction Memory
	ScalarMemory,
	/// Vector ALU Operation
	VectorALU,
	/// Vector Instruction Memory Read
	VectorMemory,
	/// Scalar Program Flow Control and Special Operations
	FlowControl,
	/// LDS and GDS
	DataShare,
	/// Vector Interpolation Operations
	VectorInterpolation,
	/// Export
	Export,
	/// Debug and Profiling Operations
	DebugProfile,
};



struct SGcnShaderInstruction
{
	uint32_t		   opcode;
	uint32_t           length;  // in bytes
	uint32_t		   encoding;
	GcnInstClass       opClass;
	GcnInstCategory    category;
};

