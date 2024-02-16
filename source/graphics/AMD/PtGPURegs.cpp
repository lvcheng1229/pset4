#include "PtGPURegs.h"

static SGPU_REGS gGpuRegs;

SGPU_REGS* GetGpuRegs()
{
    return &gGpuRegs;
}
