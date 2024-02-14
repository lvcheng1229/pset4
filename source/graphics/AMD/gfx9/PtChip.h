#pragma once
#include "chip/gfx9_plus_merged_offset.h"
#include "chip/gfx9_plus_merged_default.h"
#include "chip/gfx9_plus_merged_enum.h"
#include "chip/gfx9_plus_merged_mask.h"
#include "chip/gfx9_plus_merged_shift.h"
#include "chip/gfx9_plus_merged_registers.h"
#include "chip/gfx9_plus_merged_typedef.h"

#include "chip/gfx9_plus_merged_f32_ce_pm4_packets.h"  // constant engine
#include "chip/gfx9_plus_merged_f32_mec_pm4_packets.h" // compute engine
#include "chip/gfx9_plus_merged_f32_me_pm4_packets.h"  // micro-engine
#include "chip/gfx9_plus_merged_f32_pfp_pm4_packets.h" // pre-fetch-parser
#include "chip/gfx9_plus_merged_pm4_it_opcodes.h"

#include "chip/gfx10_sq_ko_reg.h"

namespace Pal
{
	namespace Gfx9
	{
		inline namespace Chip
		{
			namespace XGfx = Gfx09;
		}
	}
}
namespace PtGfx = Pal::Gfx9;


using PM4_PT_TYPE_3_HEADER = PtGfx::PM4_ME_TYPE_3_HEADER;
using SPI_SHADER_PGM_RSRC2_VS = PtGfx::SPI_SHADER_PGM_RSRC2_VS;
using SPI_SHADER_PGM_RSRC2_PS = PtGfx::SPI_SHADER_PGM_RSRC2_PS;
using SPI_PS_INPUT_ENA = PtGfx::SPI_PS_INPUT_ENA;

using PM4_HEADER_COMMON = PM4_PT_TYPE_3_HEADER;

#define PM4_TYPE_0 (PtGfx::PM4_TYPE_0)
#define PM4_TYPE_2 (PtGfx::PM4_TYPE_2)
#define PM4_TYPE_3 (PtGfx::PM4_TYPE_3)




