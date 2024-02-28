#include <assert.h>
#include "PtGPURegs.h"

static SGPU_REGS gGpuRegs;



SGPU_REGS* GetGpuRegs()
{
    return &gGpuRegs;
}

SGPU_REGS::SGPU_REGS()
{
    SPI.VS.RSRC3.u32All = 0x001701FD;
    SPI.VS.OUT_CNTL.u32All = 0x0000001C;

    SPI.PS.RSRC3.u32All = 0x001701FF;
}

void* CBufferResourceDesc::GetBaseAddress()const
{
    uintptr_t baseAddr = m_bufferSrd.word1.bitfields.BASE_ADDRESS_HI;
    baseAddr <<= 32;
    baseAddr |= m_bufferSrd.word0.bitfields.BASE_ADDRESS;
    return (void*)baseAddr;
}

uint32_t CBufferResourceDesc::GetStride()const
{
    return m_bufferSrd.word1.bitfields.STRIDE;
}

uint32_t CBufferResourceDesc::GetSize() const
{
    uint32_t stride = m_bufferSrd.word1.bitfields.STRIDE;
    uint32_t numElements = m_bufferSrd.word2.bitfields.NUM_RECORDS;
    return stride ? numElements * stride : numElements;
}

uint32_t GetBufferDataFormatSizeInByte(PtGfx::BUF_DATA_FORMAT dataFormat)
{
	switch (dataFormat)
	{
	case PtGfx::BUF_DATA_FORMAT_8:return 1;
	case PtGfx::BUF_DATA_FORMAT_16:return 2;
	case PtGfx::BUF_DATA_FORMAT_8_8:return 2;
	case PtGfx::BUF_DATA_FORMAT_32:return 4;
	case PtGfx::BUF_DATA_FORMAT_16_16:return 4;
	case PtGfx::BUF_DATA_FORMAT_10_11_11:return 4;
	case PtGfx::BUF_DATA_FORMAT_11_11_10:return 4;
	case PtGfx::BUF_DATA_FORMAT_10_10_10_2:return 4;
	case PtGfx::BUF_DATA_FORMAT_2_10_10_10:return 4;
	case PtGfx::BUF_DATA_FORMAT_8_8_8_8:return 4;
	case PtGfx::BUF_DATA_FORMAT_32_32:return 8;
	case PtGfx::BUF_DATA_FORMAT_16_16_16_16:return 8;
	case PtGfx::BUF_DATA_FORMAT_32_32_32:return 12;
	case PtGfx::BUF_DATA_FORMAT_32_32_32_32:return 16;
	default:assert(false);
	}
	return 0;
}

uint32_t GetImageDataFormatSizeInByte(PtGfx::IMG_DATA_FORMAT dataFormat)
{
	switch (dataFormat)
	{
	case PtGfx::IMG_DATA_FORMAT_8: return 1;
	case PtGfx::IMG_DATA_FORMAT_16:return 2;
	case PtGfx::IMG_DATA_FORMAT_8_8:return 2;
	case PtGfx::IMG_DATA_FORMAT_32:return 4;
	case PtGfx::IMG_DATA_FORMAT_16_16:return 4;
	case PtGfx::IMG_DATA_FORMAT_10_11_11:return 4;
	case PtGfx::IMG_DATA_FORMAT_11_11_10:return 4;
	case PtGfx::IMG_DATA_FORMAT_10_10_10_2:return 4;
	case PtGfx::IMG_DATA_FORMAT_2_10_10_10:return 4;
	case PtGfx::IMG_DATA_FORMAT_8_8_8_8:return 4;
	case PtGfx::IMG_DATA_FORMAT_32_32:return 8;
	case PtGfx::IMG_DATA_FORMAT_16_16_16_16:return 8;
	case PtGfx::IMG_DATA_FORMAT_32_32_32:return 12;
	case PtGfx::IMG_DATA_FORMAT_32_32_32_32:return 16;
	default :
		assert(false);
		return 0;
	}
	return 0;
}

void* CTextureResourceDesc::GetBaseAddress() const
{
	//uintptr_t baseAddr = m_imageSrd.word1.bitfields.BASE_ADDRESS_HI;
	//baseAddr <<= 32;
	//baseAddr |= m_imageSrd.word0.bitfields.BASE_ADDRESS;

	uintptr_t baseAddr = m_imageSrd.word0.bitfields.BASE_ADDRESS;
	baseAddr <<= 8;
	return (void*)baseAddr;
}
