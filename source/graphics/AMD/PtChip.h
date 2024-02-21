#pragma once
#include "gfx6/chip/si_ci_vi_merged_enum.h"
#include "gfx6/chip/si_ci_vi_merged_offset.h"
#include "gfx6/chip/si_ci_vi_merged_pm4_it_opcodes.h"
#include "gfx6/chip/si_ci_vi_merged_pm4defs.h"
#include "gfx6/chip/si_ci_vi_merged_registers.h"
#include "gfx6/chip/si_ci_vi_merged_typedef.h"

//#include "gfx9/gfx9_plus_merged_f32_ce_pm4_packets.h"  // constant engine
//#include "gfx9/gfx9_plus_merged_f32_mec_pm4_packets.h" // compute engine
//#include "gfx9/gfx9_plus_merged_f32_me_pm4_packets.h"  // micro-engine
//#include "gfx9/gfx9_plus_merged_f32_pfp_pm4_packets.h" // pre-fetch-parser
//#include "gfx9/gfx9_plus_merged_pm4_it_opcodes.h"
//
//#include "gfx9/gfx10_sq_ko_reg.h"

namespace Pal
{
	namespace Gfx6
	{
		inline namespace Chip
		{
            typedef struct _PM4CMDSETDATA
            {
                union
                {
                    struct
                    {
                        unsigned int    regOffset : 16;  ///< offset in DWords from the register base address
                        unsigned int    reserved1 : 12;  ///< Program to zero
                        unsigned int    index : 4;   ///< Index for UCONFIG/CONTEXT on CI+
                    };
                    unsigned int        ordinal2;
                };
            } PM4CMDSETDATA, * PPM4CMDSETDATA;
		}
	}
}
namespace PtGfx = Pal::Gfx6;


using PM4_PT_TYPE_3_HEADER = PtGfx::PM4_TYPE_3_HEADER;
using SPI_SHADER_PGM_RSRC2_VS = PtGfx::SPI_SHADER_PGM_RSRC2_VS;
using SPI_SHADER_PGM_RSRC2_PS = PtGfx::SPI_SHADER_PGM_RSRC2_PS;
using SPI_PS_INPUT_ENA = PtGfx::SPI_PS_INPUT_ENA;

using PM4_HEADER_COMMON = PM4_PT_TYPE_3_HEADER;

#define PM4_TYPE_0 (PtGfx::PM4_TYPE_0)
#define PM4_TYPE_2 (PtGfx::PM4_TYPE_2)
#define PM4_TYPE_3 (PtGfx::PM4_TYPE_3)


// Buffer resource descriptor structure
// V# Vertex Descriptor Buffer
struct GfxBufferSrd
{
    PtGfx::SQ_BUF_RSRC_WORD0 word0;
    PtGfx::SQ_BUF_RSRC_WORD1 word1;
    PtGfx::SQ_BUF_RSRC_WORD2 word2;
    PtGfx::SQ_BUF_RSRC_WORD3 word3;
};

// Image resource descriptor structure
// T# Texture Descriptor Buffer
struct GfxImageSrd
{
    PtGfx::SQ_IMG_RSRC_WORD0 word0;
    PtGfx::SQ_IMG_RSRC_WORD1 word1;
    PtGfx::SQ_IMG_RSRC_WORD2 word2;
    PtGfx::SQ_IMG_RSRC_WORD3 word3;
    PtGfx::SQ_IMG_RSRC_WORD4 word4;
    PtGfx::SQ_IMG_RSRC_WORD5 word5;
    PtGfx::SQ_IMG_RSRC_WORD6 word6;
    PtGfx::SQ_IMG_RSRC_WORD7 word7;
};




