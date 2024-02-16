#pragma once
#include <stdint.h>

#define PM4_HEADER_BUILD(lenDw, op, priv) (((uint32_t)(((((uint16_t)lenDw) << 16) + 0x3FFE0000) | 0xC0000000)) | ((uint8_t)(op)) << 8 | ((uint8_t)(priv)))
#define PM4_PRIV(token) ((uint8_t)(((uint32_t)(token)) & 0xFF))
#define PM4_TYPE(token) (((uint32_t)(token) >> 30) & 3)
#define PM4_LENGTH_DW(token) ((((uint32_t)(token) >> 16) & 0x3FFF) + 2)

#define IT_OP_CUS 0x000000FF

enum IT_OpCodeCustom : uint8_t
{
	OP_CUS_INITIALIZE_DEFAULT_HARDWARE_STATE = 0x00,
	OP_CUS_INITIALIZE_TO_DEFAULT_CONTEXT_STATE,
	OP_CUS_SET_VS_SHADER ,
	OP_CUS_SET_PS_SHADER,
	OP_CUS_DRAW_INDEX,
	OP_CUS_WAIT_UNTIL_SAFE_FOR_RENDERING,
	OP_CUS_UPDATE_VS_SHADER,
};

