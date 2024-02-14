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

struct SGcnShaderInstruction
{
	uint32_t opcode; 
};