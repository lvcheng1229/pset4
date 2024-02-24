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
