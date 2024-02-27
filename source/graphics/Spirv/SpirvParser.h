#pragma once
#include <stdint.h>

//OnSourceExtension
struct SSpirvHeader
{
	uint32_t m_magic;
	
	uint16_t m_version_minjor;
	uint16_t m_version_major;
	
	uint16_t m_tool_version;
	uint16_t m_tool_id;
	
	uint32_t m_bound;
	uint32_t m_reserved;
};

class CPtSpirvParser
{
public:
	void SetSpvSource(uint8_t* spvSrc);
	void Parse();
private:
	uint8_t* m_spvSrc;
};