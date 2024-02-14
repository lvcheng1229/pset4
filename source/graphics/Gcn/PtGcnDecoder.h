#pragma once
#include <stdint.h>
#include "PtGcnInstruction.h"

class CGcnCodeReader
{
public:
	CGcnCodeReader(const uint32_t* ptr, const uint32_t* end) :
		m_ptr(ptr), m_end(end)
	{
	}

	inline uint32_t operator[](uint32_t index) { return m_ptr[index]; }
	inline uint32_t ReadU32() { return *(m_ptr++); };
	inline uint64_t ReadU64() { uint64_t value = *(uint64_t*)m_ptr; m_ptr += 2; return value; };
	inline bool IsEnd() { return m_ptr == m_end; };

private:
	const uint32_t* m_ptr = nullptr;
	const uint32_t* m_end = nullptr;
};

class CGcnDecodeContext
{
public:
	void DecodeInstruction(CGcnCodeReader& code);

	inline const SGcnShaderInstruction& GetInstruction() const { return m_gcnInstruction; }
private:
	uint32_t GetInstructionEncoding(uint32_t token);
	uint32_t GetEncodingLength(uint32_t instructionEnc);

private:

	SGcnShaderInstruction m_gcnInstruction;
};