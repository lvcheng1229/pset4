#include <assert.h>
#include "PtGnmDriver.h"
static CPtGnmDriver* gPtGnmDriver = nullptr;
CPtGnmDriver* GetPtGnmDriver()
{
	if (gPtGnmDriver == nullptr)
	{
		gPtGnmDriver = new CPtGnmDriver();
	}
	return gPtGnmDriver;
}

void CPtGnmDriver::SubmitAndFlipCommandBuffers(uint32_t count, void* dcbGpuAddrs[], uint32_t* dcbSizesInBytes, void* ccbGpuAddrs[], uint32_t* ccbSizesInBytes, uint32_t videoOutHandle, uint32_t displayBufferIndex, uint32_t flipMode, int64_t flipArg)
{
	assert(count == 1);
	m_ptGnmTranslator.TranslateAndDispatchCmd(dcbGpuAddrs[0], dcbSizesInBytes[0]);
}
