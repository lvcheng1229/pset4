#pragma once
#include "overridemodule\PsetLibraryCommon.h" 
#include "graphics\Gnm\GnmStructure.h"

int PSET_SYSV_ABI Pset_sceGnmDrawInitDefaultHardwareState350(int32_t* cmdBuffer, int32_t numDwords);
int PSET_SYSV_ABI Pset_sceGnmInsertWaitFlipDone(uint32_t* cmdBuffer, uint32_t numDwords, int videoOutHandle, uint32_t displayBufferIndex);
int PSET_SYSV_ABI Pset_sceGnmSetVsShader(uint32_t* cmdBuffer, uint32_t numDwords, const VsStageRegisters* vsRegs, uint32_t shaderModifier);
int PSET_SYSV_ABI Pset_sceGnmSetPsShader350(uint32_t* cmdBuffer, uint32_t numDwords, const PsStageRegisters* psRegs);
int PSET_SYSV_ABI Pset_sceGnmDrawIndex(uint32_t* cmdBuffer, uint32_t numDwords, uint32_t indexCount, const void* indexAddr, uint32_t predAndMod, uint32_t inlineMode);
int PSET_SYSV_ABI Pset_sceGnmUpdateVsShader(uint32_t* cmdBuffer, uint32_t numDwords, const VsStageRegisters* vsRegs, uint32_t shaderModifier);
int PSET_SYSV_ABI Pset_sceGnmSubmitAndFlipCommandBuffers(uint32_t count, void* dcbGpuAddrs[], uint32_t* dcbSizesInBytes, void* ccbGpuAddrs[], uint32_t* ccbSizesInBytes, uint32_t videoOutHandle, uint32_t displayBufferIndex, uint32_t flipMode, int64_t flipArg);
int PSET_SYSV_ABI Pset_sceGnmSubmitDone(void);



