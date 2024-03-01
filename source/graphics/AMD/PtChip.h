#pragma once
#include <stdint.h>

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

// see gfx9_plus_merged_enum.h - GB_TILE_MODE0_GEN
static constexpr uint32_t gTileMode[32] =
{
    0x90800310,	// GB_TILE_MODE0  0x00 kTileModeDepth_2dThin_64	      am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Depth    ss=1
    0x90800B10,	// GB_TILE_MODE1  0x01 kTileModeDepth_2dThin_128   	  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts= 128  mtm=Depth    ss=1
    0x90801310,	// GB_TILE_MODE2  0x02 kTileModeDepth_2dThin_256   	  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts= 256  mtm=Depth    ss=1
    0x90801B10,	// GB_TILE_MODE3  0x03 kTileModeDepth_2dThin_512   	  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts= 512  mtm=Depth    ss=1
    0x90802310,	// GB_TILE_MODE4  0x04 kTileModeDepth_2dThin_1K		  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=1024  mtm=Depth    ss=1
    0x90800308,	// GB_TILE_MODE5  0x05 kTileModeDepth_1dThin       	  am=1dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Depth    ss=1
    0x90801318,	// GB_TILE_MODE6  0x06 kTileModeDepth_2dThinPrt_256	  am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts= 256  mtm=Depth    ss=1
    0x90802318,	// GB_TILE_MODE7  0x07 kTileModeDepth_2dThinPrt_1K 	  am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=1024  mtm=Depth    ss=1
    0x90000304,	// GB_TILE_MODE8  0x08 kTileModeDisplay_LinearAligned am=LinearAligned    pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Display  ss=1
    0x90000308,	// GB_TILE_MODE9  0x09 kTileModeDisplay_1dThin        am=1dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Display  ss=1
    0x92000310,	// GB_TILE_MODE10 0x0A kTileModeDisplay_2dThin        am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Display  ss=2
    0x92000294,	// GB_TILE_MODE11 0x0B kTileModeDisplay_ThinPrt       am=TiledThinPrt     pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Display  ss=2
    0x92000318,	// GB_TILE_MODE12 0x0C kTileModeDisplay_2dThinPrt     am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Display  ss=2
    0x90400308,	// GB_TILE_MODE13 0x0D kTileModeThin_1dThin   		  am=1dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thin     ss=1
    0x92400310,	// GB_TILE_MODE14 0x0E kTileModeThin_2dThin   		  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thin     ss=2
    0x924002B0,	// GB_TILE_MODE15 0x0F kTileModeThin_3dThin   		  am=3dTiledThin      pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thin     ss=2
    0x92400294,	// GB_TILE_MODE16 0x10 kTileModeThin_ThinPrt  		  am=TiledThinPrt     pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thin     ss=2
    0x92400318,	// GB_TILE_MODE17 0x11 kTileModeThin_2dThinPrt		  am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thin     ss=2
    0x9240032C,	// GB_TILE_MODE18 0x12 kTileModeThin_3dThinPrt		  am=3dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thin     ss=2
    0x9100030C,	// GB_TILE_MODE19 0x13 kTileModeThick_1dThick   	  am=1dTiledThick     pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thick    ss=1
    0x9100031C,	// GB_TILE_MODE20 0x14 kTileModeThick_2dThick   	  am=2dTiledThick     pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thick    ss=1
    0x910002B4,	// GB_TILE_MODE21 0x15 kTileModeThick_3dThick   	  am=3dTiledThick     pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thick    ss=1
    0x910002A4,	// GB_TILE_MODE22 0x16 kTileModeThick_ThickPrt  	  am=TiledThickPrt    pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thick    ss=1
    0x91000328,	// GB_TILE_MODE23 0x17 kTileModeThick_2dThickPrt	  am=2dTiledThickPrt  pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thick    ss=1
    0x910002BC,	// GB_TILE_MODE24 0x18 kTileModeThick_3dThickPrt	  am=3dTiledThickPrt  pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thick    ss=1
    0x91000320,	// GB_TILE_MODE25 0x19 kTileModeThick_2dXThick  	  am=2dTiledXThick    pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Thick    ss=1
    0x910002B8,	// GB_TILE_MODE26 0x1A kTileModeThick_3dXThick  	  am=3dTiledXThick    pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Thick    ss=1
    0x90C00308,	// GB_TILE_MODE27 0x1B kTileModeRotated_1dThin   	  am=1dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Rotated  ss=1
    0x92C00310,	// GB_TILE_MODE28 0x1C kTileModeRotated_2dThin   	  am=2dTiledThin      pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Rotated  ss=2
    0x92C00294,	// GB_TILE_MODE29 0x1D kTileModeRotated_ThinPrt  	  am=TiledThinPrt     pipe/alt=P8_32x32_8x16 /P16  ts=  64  mtm=Rotated  ss=2
    0x92C00318,	// GB_TILE_MODE30 0x1E kTileModeRotated_2dThinPrt	  am=2dTiledThinPrt   pipe/alt=P8_32x32_16x16/P16  ts=  64  mtm=Rotated  ss=2
    0x00000000, // GB_TILE_MODE31 0x1F kTileModeDisplay_LinearGeneral am=LinearGeneral    pipe/alt=P2            / P2  ts=  64  mtm=Display  ss=1

};



