#pragma once
#include <stdint.h>
#include "PtGcnCommandDefs.h"


enum EShaderStage :uint32_t
{
	eShaderStageVS = 0,
	eShaderStagePS = 1,
	eShaderStateCount = 2
};

#define GnmMaxResourceCount        128 ///< PSSL compiler limit is 128, Default value is 16
#define GnmMaxRwResourceCount      16  ///< PSSL compiler limit is 16, Default value is 16
#define GnmMaxSamplerCount         16  ///< PSSL compiler limit is 16, Default value is 16
#define GnmMaxVertexBufferCount    32  ///< PSSL compiler limit is 32, Default value is 16
#define GnmMaxConstantBufferCount  20  ///< PSSL compiler limit is 20, Default value is 20 Note: Becaus
#define GnmMaxStreamOutBufferCount 4   ///< PSSL compiler limit is 4, Default value is 4
#define GnmMaxUserDataCount        16  ///< PSSL compiler limit is 16, count not tracked by the InputRe
#define GnmMaxSrtUserDataCount     16  ///< PSSL compiler limit is 16, count not tracked by the InputRe
#define GnmMaxResourceBufferCount  4   ///< Maximum number for supported splits for the resource buffer
#define GnmMaxPsInputUsageCount    32  ///< Maximum number of interpolants a PS Stage can receive
