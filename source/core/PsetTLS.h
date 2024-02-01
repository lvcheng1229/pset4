#pragma once
#include <stdint.h>
struct STLSBlock
{
	uint32_t m_tmpStatrt;
	uint32_t m_tmpSize; // init size
	uint32_t m_fullSize;
	uint32_t m_align;
	uint32_t m_index; 
	uint32_t m_offset;
};