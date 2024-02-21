#pragma once
#include <stdint.h>
#include "graphics\AMD\PtChip.h"
struct SVertexElement
{
	uint32_t location;
	uint32_t binding;
	uint32_t offset;

	PtGfx::BUF_DATA_FORMAT m_vertexDataFormat;
	PtGfx::BUF_NUM_FORMAT m_vertexNumFormat;
};

struct SVertexBinding
{
	uint32_t m_stride = 0;
};