#pragma once
#include "PtGnmTranslator.h"
#include "graphics\Gnm\PtGnmCode.h"

class CPtGnmDriver
{
public:
	void Submitdone();
	void SubmitAndFlipCommandBuffers(
		uint32_t count, void* dcbGpuAddrs[], uint32_t* dcbSizesInBytes,
		void* ccbGpuAddrs[], uint32_t* ccbSizesInBytes, uint32_t videoOutHandle,
		uint32_t displayBufferIndex, uint32_t flipMode, int64_t flipArg);

private:
	CPtGnmTranslator m_ptGnmTranslator;
};

CPtGnmDriver* GetPtGnmDriver();