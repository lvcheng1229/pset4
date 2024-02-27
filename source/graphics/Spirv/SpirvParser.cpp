#include "SpirvParser.h"

void CPtSpirvParser::SetSpvSource(uint8_t* spvSrc)
{
	m_spvSrc = spvSrc;
}

void CPtSpirvParser::Parse()
{
	SSpirvHeader* spvHeader = (SSpirvHeader*)m_spvSrc;
}
