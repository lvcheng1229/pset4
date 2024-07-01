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

	PtGfx::SPI_PS_INPUT_CNTL_0 INPUT_CNTL[32];
};

struct SShaderProcessorInput
{
	SSPIVS VS;
	SSPIPS PS;
};

struct SDepth
{
	PtGfx::DB_RENDER_CONTROL RENDER_CONTROL; //depth/stencil clear/compress
	PtGfx::DB_DEPTH_CONTROL DEPTH_CONTROL; //depth stencil
	
	// unused reigisters are not listed here
	PtGfx::DB_DEPTH_VIEW  DEPTH_VIEW;
	PtGfx::DB_DEPTH_INFO DEPTH_INFO;
};

struct SRenderTarget
{
	PtGfx::CB_COLOR0_BASE BASE;
	PtGfx::CB_COLOR0_PITCH PITCH;
	PtGfx::CB_COLOR0_SLICE SLICE;
	PtGfx::CB_COLOR0_VIEW VIEW;
	PtGfx::CB_COLOR0_INFO INFO;
	PtGfx::CB_COLOR0_ATTRIB ATTRIB;
	PtGfx::CB_COLOR0_DCC_CONTROL DCC_CONTROL;
	PtGfx::CB_COLOR0_CMASK CMASK;
	PtGfx::CB_COLOR0_CMASK_SLICE CMASK_SLICE;
	PtGfx::CB_COLOR0_FMASK FMASK;
	PtGfx::CB_COLOR0_FMASK_SLICE FMASK_SLICE;
	PtGfx::CB_COLOR0_CLEAR_WORD0 CLEAR_WORD0;
	PtGfx::CB_COLOR0_CLEAR_WORD1 CLEAR_WORD1;
	PtGfx::CB_COLOR0_DCC_BASE DCC_BASE;
	uint32_t PADDING;
};

struct SVGT_DMA
{
	PtGfx::VGT_INDEX_TYPE_MODE INDEX_TYPE;
};

class SGPU_REGS
{
public:
	SGPU_REGS();

	SShaderProcessorInput SPI;

	SVGT_DMA VGT_DMA;

	SDepth DEPTH;

	PtGfx::VGT_NUM_INSTANCES VGT_NUM_INSTANCES;
	
	SRenderTarget RENDER_TARGET[8];
	PtGfx::CB_TARGET_MASK TARGET_MASK;

	PtGfx::CB_BLEND0_CONTROL CB_BLEND_CONTROL[8]; // color blend control
	

	PtGfx::CB_COLOR_CONTROL CB_COLOR_CONTROL; // pipeline color blend control

	PtGfx::PA_SU_SC_MODE_CNTL SC_MODE_CNTL;	// cull face / poly mode
	PtGfx::PA_SC_MODE_CNTL_0 SC_MODE_CNTL_0; // msaa
	PtGfx::PA_SC_MODE_CNTL_1 SC_MODE_CNTL_1;
};

class CBufferResourceDesc
{
public:
	void* GetBaseAddress()const;
	uint32_t GetStride()const;
	uint32_t GetSize()const;
	GfxBufferSrd m_bufferSrd;
};

class CTextureResourceDesc
{
public:
	void* GetBaseAddress()const;
	inline uint32_t GetTextureWidth()const { return m_imageSrd.word2.bitfields.WIDTH + 1u; };
	inline uint32_t GetTextureHeight()const { return m_imageSrd.word2.bitfields.HEIGHT + 1u; };
	//uint32_t GetStride()const;
	//uint32_t GetSize()const;
	GfxImageSrd m_imageSrd;
};

uint32_t GetBufferDataFormatSizeInByte(PtGfx::BUF_DATA_FORMAT dataFormat);
uint32_t GetImageDataFormatSizeInByte(PtGfx::IMG_DATA_FORMAT dataFormat);

inline void* GetFetchAddress(uint32_t lo, uint32_t hi) { return (void*)(uintptr_t(hi) << 32 | (uintptr_t(lo) & ~(0x3u))); };
inline void* GetCodeAddress(uint32_t lo, uint32_t hi) { return (void*)(uintptr_t(hi) << 40 | uintptr_t(lo) << 8); }


SGPU_REGS* GetGpuRegs();