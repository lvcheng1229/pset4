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
}
