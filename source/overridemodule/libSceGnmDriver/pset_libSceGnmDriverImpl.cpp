#include "pset_libSceGnmDriverImpl.h"
#include "graphics\Sce\PtVideoOut.h"
#include "graphics\Gnm\PtGnmDriver.h"
#include <Windows.h>

static HANDLE gIdleEvent;

int PSET_SYSV_ABI Pset_sceGnmDrawInitDefaultHardwareState350(int32_t* cmdBuffer, int32_t numDwords)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceGnmDrawInitDefaultHardwareState350");
	const uint32_t initCmdSize = sizeof(SGnmCmdDrawInitDefaultHardwareState) / sizeof(uint32_t);
	SGnmCmdDrawInitDefaultHardwareState* initParam = (SGnmCmdDrawInitDefaultHardwareState*)cmdBuffer;
	initParam->opcode = PM4_HEADER_BUILD(initCmdSize, IT_OP_CUS, OP_CUS_INITIALIZE_DEFAULT_HARDWARE_STATE);
	memset(initParam->reserved, 0, sizeof(initParam->reserved));
	return initCmdSize;
}

int PSET_SYSV_ABI Pset_sceGnmInsertWaitFlipDone(uint32_t* cmdBuffer, uint32_t numDwords, int videoOutHandle, uint32_t displayBufferIndex)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceGnmInsertWaitFlipDone");
	const uint32_t cmdSize = sizeof(SGnmCmdWaitFlipDone) / sizeof(uint32_t);
	assert(cmdSize == numDwords);
	SGnmCmdWaitFlipDone* param = (SGnmCmdWaitFlipDone*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(cmdSize, IT_OP_CUS, OP_CUS_WAIT_UNTIL_SAFE_FOR_RENDERING);
	param->videoOutHandle = videoOutHandle;
	param->displayBufferIndex = displayBufferIndex;
	memset(param->reserved, 0, sizeof(param->reserved));
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceGnmSetVsShader(uint32_t* cmdBuffer, uint32_t numDwords, const VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceGnmSetVsShader");
	const uint32_t paramSize = sizeof(GnmCmdSetVSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdSetVSShader* param = (GnmCmdSetVSShader*)cmdBuffer;
	memset(param,0,sizeof(GnmCmdSetVSShader));
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_OP_CUS, OP_CUS_SET_VS_SHADER);
	param->modifier = shaderModifier;
	if (vsRegs != nullptr)
	{
		memcpy(&param->vsRegs, vsRegs, sizeof(VsStageRegisters));
	}
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceGnmSetPsShader350(uint32_t* cmdBuffer, uint32_t numDwords, const PsStageRegisters* psRegs)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceGnmSetPsShader350");
	const uint32_t paramSize = sizeof(GnmCmdSetPSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdSetPSShader* param = (GnmCmdSetPSShader*)cmdBuffer;
	memset(param, 0, sizeof(GnmCmdSetPSShader));
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_OP_CUS, OP_CUS_SET_PS_SHADER);
	if (psRegs != nullptr)
	{
		memcpy(&param->psRegs, psRegs, sizeof(PsStageRegisters));
	}
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceGnmDrawIndex(uint32_t* cmdBuffer, uint32_t numDwords, uint32_t indexCount, const void* indexAddr, uint32_t predAndMod, uint32_t inlineMode)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceGnmDrawIndex");
	const uint32_t paramSize = sizeof(GnmCmdDrawIndex) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdDrawIndex* param = (GnmCmdDrawIndex*)cmdBuffer;
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_OP_CUS, OP_CUS_DRAW_INDEX);
	param->indexCount = indexCount;
	param->indexAddr = (uintptr_t)indexAddr;
	param->predAndMod = predAndMod;
	param->inlineMode = inlineMode;
	memset(param->reserved, 0, sizeof(param->reserved));
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceGnmUpdateVsShader(uint32_t* cmdBuffer, uint32_t numDwords, const VsStageRegisters* vsRegs, uint32_t shaderModifier)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceGnmUpdateVsShader");
	const uint32_t paramSize = sizeof(GnmCmdSetVSShader) / sizeof(uint32_t);
	assert(paramSize == numDwords);
	GnmCmdSetVSShader* param = (GnmCmdSetVSShader*)cmdBuffer;
	memset(param,0,sizeof(GnmCmdSetVSShader));
	param->opcode = PM4_HEADER_BUILD(paramSize, IT_OP_CUS, OP_CUS_UPDATE_VS_SHADER);
	param->modifier = shaderModifier;
	if (vsRegs != nullptr)
	{
		memcpy(&param->vsRegs, vsRegs, sizeof(VsStageRegisters));
	}
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceGnmSubmitAndFlipCommandBuffers(uint32_t count, void* dcbGpuAddrs[], uint32_t* dcbSizesInBytes, void* ccbGpuAddrs[], uint32_t* ccbSizesInBytes, uint32_t videoOutHandle, uint32_t displayBufferIndex, uint32_t flipMode, int64_t flipArg)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceGnmSubmitAndFlipCommandBuffers");
	GetPtGnmDriver()->SubmitAndFlipCommandBuffers(count, dcbGpuAddrs, dcbSizesInBytes, ccbGpuAddrs, ccbSizesInBytes, videoOutHandle, displayBufferIndex, flipMode, flipArg);
	
	//gIdleEvent = CreateEventA(nullptr, true, true, nullptr);
	//ResetEvent(gIdleEvent);
	
	//HANDLE     processHandle = GetCurrentProcess();
	//FILETIME createTime, exitTime, kernelTime, userTime;
	//GetProcessTimes(processHandle, &createTime, &exitTime, &kernelTime, &userTime); //ms
	//
	//uint64_t lpPerformanceCount;
	//QueryPerformanceCounter((LARGE_INTEGER*)&lpPerformanceCount);
	//
	//SVblankStatus& blankStatus =  GetVideoOut(videoOutHandle)->GetBlankStatus();
	//blankStatus.m_processTime = *(uint64_t*)(&userTime);
	//blankStatus.m_timeStampCounter = lpPerformanceCount;
	
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceGnmSubmitDone(void)
{
	GetPtGnmDriver()->Submitdone();
	//WaitForSingleObject(gIdleEvent,);
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceGnmSubmitDone");
	return PSET_OK;
}