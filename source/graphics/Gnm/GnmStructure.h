#include <stdint.h>

//TODO: see  fpcs4
// Shader registers
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

struct SGnmCmdDrawInitDefaultHardwareState
{
	uint32_t opcode;
	uint32_t reserved[255];
};

typedef enum EmbeddedVsShader
{
	kEmbeddedVsShaderFullScreen = 0,
	kNumEmbeddedVsShaders
} EmbeddedVsShader;

struct SGnmCmdWaitFlipDone
{
	uint32_t opcode;
	uint32_t videoOutHandle;
	uint32_t displayBufferIndex;
	uint32_t reserved[4];
};

struct GnmCmdVSShader
{
	uint32_t              opcode;
	VsStageRegisters vsRegs;
	EmbeddedVsShader      shaderId;
	uint32_t              modifier;
	uint32_t              reserved[19];
};

struct GnmCmdPSShader
{
	uint32_t              opcode;
	PsStageRegisters	  psRegs;
	uint32_t              reserved[27];
};

enum GnmEnumDrawIndexInlineMode : uint32_t
{
	INLINE_MODE_NOINLINE = 1,
	INLINE_MODE_INLINE = 2,
};

struct GnmCmdDrawIndex
{
	uint32_t                   opcode;
	uint32_t                   indexCount;
	uintptr_t                  indexAddr;
	uint32_t                   predAndMod;
	GnmEnumDrawIndexInlineMode inlineMode;
	uint32_t                   reserved[4];
};
