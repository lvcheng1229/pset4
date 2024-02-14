#pragma once
#include <stdint.h>
#include "PtGcnShaderInfo.h"
#include "PtGcnInstruction.h"

class CPtGcnModule
{
public:
	CPtGcnModule(const uint8_t* code);
	void Compile();
private:
	std::vector<SGcnShaderInstruction> DecodeShaderInternal(const uint32_t* start, const uint32_t* end);

	CGcnHeader m_header;
	const uint8_t* m_code;
};