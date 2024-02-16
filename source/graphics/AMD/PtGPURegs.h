#pragma once
#include <stdint.h>
#include "graphics\AMD\PtChip.h"


struct SSPIVS
{
	uint32_t LO;
	uint32_t HI;

	PtGfx::SPI_SHADER_PGM_RSRC1_VS RSRC1; //register record 1
	PtGfx::SPI_SHADER_PGM_RSRC2_VS RSRC2; //register record 2
	PtGfx::SPI_SHADER_PGM_RSRC3_VS RSRC3; //register record 2

	PtGfx::SPI_VS_OUT_CONFIG OUT_CONFIG;
	PtGfx::SPI_SHADER_POS_FORMAT POS_FORMAT;
	PtGfx::PA_CL_VS_OUT_CNTL OUT_CNTL;

	PtGfx::SPI_SHADER_USER_DATA_VS_0 USER_DATA[16];
};
static_assert(sizeof(PtGfx::SPI_SHADER_PGM_RSRC1_VS) == sizeof(uint32_t));

struct SShaderProcessorInput
{
	SSPIVS VS;

};

class SGPU_REGS
{
public:
	PtGfx::VGT_NUM_INSTANCES VGT_NUM_INSTANCES;
	SShaderProcessorInput SPI;
};

SGPU_REGS* GetGpuRegs();