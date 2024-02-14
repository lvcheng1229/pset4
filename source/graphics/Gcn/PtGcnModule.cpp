#include "PtGcnModule.h"
#include "PtGcnDecoder.h"

CPtGcnModule::CPtGcnModule(const uint8_t* code)
	:m_header(code),
	m_code(code)
{
}

void CPtGcnModule::Compile()
{
	const uint32_t* start = reinterpret_cast<const uint32_t*>(m_code);
	const uint32_t* end = reinterpret_cast<const uint32_t*>(m_code + m_header.GetLength());
	DecodeShaderInternal(start, end);
}

std::vector<SGcnShaderInstruction> CPtGcnModule::DecodeShaderInternal(const uint32_t* start, const uint32_t* end)
{
	CGcnCodeReader gcnCodeReader(start, end);
	CGcnDecodeContext gcnDecoder;

	std::vector<SGcnShaderInstruction> instructionList;
	instructionList.reserve(m_header.GetLength() / sizeof(uint32_t));
	while (gcnCodeReader.IsEnd())
	{
		gcnDecoder.DecodeInstruction(gcnCodeReader);
		instructionList.emplace_back(gcnDecoder.GetInstruction());
	}
	return instructionList;
}
