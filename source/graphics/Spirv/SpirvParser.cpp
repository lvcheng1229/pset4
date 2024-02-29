#include <assert.h>
#include "spirv.h"
#include "SpirvParser.h"

void CPtSpirvParser::SetSpvSource(uint8_t* spvSrc, uint32_t size)
{
	m_spvSrc = spvSrc;
	m_size = size;
}

void CPtSpirvParser::ModifyFragmentShaderSpirvSet()
{
	SSpirvHeader* spvHeader = (SSpirvHeader*)m_spvSrc;

	uint8_t* data = (uint8_t*)(spvHeader + 1);
	m_size = m_size - sizeof(SSpirvHeader);

	while (m_size > 0)
	{
		SSpirvInstruction* spvInstruction = (SSpirvInstruction*)data;

		if (spvInstruction->op == spv::OpDecorate && spvInstruction->count >= 4)
		{
			uint32_t d = *((uint32_t*)data + 2);
			if (d == spv::DecorationDescriptorSet)
			{
				assert(*((uint32_t*)data + 3) == 0);
				*((uint32_t*)data + 3) = 1;
			}
		}

		uint32_t step = spvInstruction->count;
		if (spvInstruction->count == 0) { step = 1; }
		data = data + (step * sizeof(uint32_t));
		m_size = m_size - step * sizeof(uint32_t);
	}
}

void CPtSpirvParser::Parse()
{
	
}
