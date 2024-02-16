#pragma once
#include <stdint.h>

struct SGnmCmdDrawInitDefaultHardwareState
{
	uint32_t opcode;
	uint32_t reserved[255];
};

struct PsStageRegisters
{
	uint32_t spiShaderPgmLoPs;
	uint32_t spiShaderPgmHiPs;
	uint32_t spiShaderPgmRsrc1Ps;
	uint32_t spiShaderPgmRsrc2Ps;
	uint32_t spiShaderZFormat;
	uint32_t spiShaderColFormat;
	uint32_t spiPsInputEna;
	uint32_t spiPsInputAddr;
	uint32_t spiPsInControl;
	uint32_t spiBarycCntl;
	uint32_t dbShaderControl;
	uint32_t cbShaderMask;

	void* getCodeAddress() const
	{
		return (void*)(uintptr_t(spiShaderPgmHiPs) << 40 | uintptr_t(spiShaderPgmLoPs) << 8);
	}
};

struct VsStageRegisters
{
	uint32_t spiShaderPgmLoVs;
	uint32_t spiShaderPgmHiVs;
	uint32_t spiShaderPgmRsrc1Vs;
	uint32_t spiShaderPgmRsrc2Vs;
	uint32_t spiVsOutConfig;
	uint32_t spiShaderPosFormat;
	uint32_t paClVsOutCntl;

	void* getCodeAddress() const
	{
		return (void*)(uintptr_t(spiShaderPgmHiVs) << 40 | uintptr_t(spiShaderPgmLoVs) << 8);
	}
};

struct GnmCmdSetVSShader
{
	uint32_t              opcode;
	VsStageRegisters	  vsRegs;
	uint32_t			  isEmbeddedFullScreen;
	uint32_t              modifier;
	uint32_t              reserved[19];
};

struct GnmCmdSetPSShader
{
	uint32_t              opcode;
	PsStageRegisters	  psRegs;
	uint32_t              reserved[27];
};

struct GnmCmdDrawIndex
{
	uint32_t                   opcode;
	uint32_t                   indexCount;
	uintptr_t                  indexAddr;
	uint32_t                   predAndMod;
	uint32_t				   inlineMode;
	uint32_t                   reserved[4];
};

struct SGnmCmdWaitFlipDone
{
	uint32_t opcode;
	uint32_t videoOutHandle;
	uint32_t displayBufferIndex;
	uint32_t reserved[4];
};