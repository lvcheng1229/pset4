#pragma once
#include <stdint.h>
#include "graphics\AMD\PtChip.h"


struct SSPIVS
{
	uint32_t LO;
	uint32_t HI;

	PtGfx::SPI_SHADER_PGM_RSRC1_VS RSRC1; //register record 1
	PtGfx::SPI_SHADER_PGM_RSRC2_VS RSRC2; //register record 2
	PtGfx::SPI_SHADER_PGM_RSRC3_VS RSRC3; //register record 3

	PtGfx::SPI_VS_OUT_CONFIG OUT_CONFIG;
	PtGfx::SPI_SHADER_POS_FORMAT POS_FORMAT;
	PtGfx::PA_CL_VS_OUT_CNTL OUT_CNTL;

	uint32_t USER_DATA[16];
};

struct SSPIPS
{
	uint32_t LO;
	uint32_t HI;

	PtGfx::SPI_SHADER_PGM_RSRC1_PS RSRC1; //register record 1
	PtGfx::SPI_SHADER_PGM_RSRC2_PS RSRC2; //register record 2
	PtGfx::SPI_SHADER_PGM_RSRC3_PS RSRC3; //register record 3

	PtGfx::SPI_SHADER_Z_FORMAT Z_FORMAT;
	PtGfx::SPI_SHADER_COL_FORMAT COL_FORMAT;
	
	PtGfx::SPI_PS_INPUT_ENA INPUT_ENA;
	PtGfx::SPI_PS_INPUT_ADDR INPUT_ADDR;
	PtGfx::SPI_PS_IN_CONTROL IN_CONTROL;
	PtGfx::SPI_BARYC_CNTL BARYC_CNTL;

	PtGfx::DB_SHADER_CONTROL SHADER_CONTROL;
	PtGfx::CB_SHADER_MASK SHADER_MASK;

	uint32_t USER_DATA[16];
};

struct SShaderProcessorInput
{
	SSPIVS VS;
	SSPIPS PS;
};

class SGPU_REGS
{
public:
	SGPU_REGS();

	PtGfx::VGT_NUM_INSTANCES VGT_NUM_INSTANCES;
	SShaderProcessorInput SPI;
};

class CBufferResourceDesc
{
public:
	void* GetBaseAddress()const;
	uint32_t GetStride()const;
	GfxBufferSrd m_bufferSrd;
};

inline void* GetFetchAddress(uint32_t lo, uint32_t hi) { return (void*)(uintptr_t(hi) << 32 | (uintptr_t(lo) & ~(0x3u))); };
inline void* GetCodeAddress(uint32_t lo, uint32_t hi) { return (void*)(uintptr_t(hi) << 40 | uintptr_t(lo) << 8); }


SGPU_REGS* GetGpuRegs();