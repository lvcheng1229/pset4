#pragma once
#include <stdint.h>

namespace Pal
{
    typedef int8_t    int8;     ///< 8-bit integer.
    typedef int16_t   int16;    ///< 16-bit integer.
    typedef int32_t   int32;    ///< 32-bit integer.
    typedef int64_t   int64;    ///< 64-bit integer.
    typedef uint8_t   uint8;    ///< Unsigned 8-bit integer.
    typedef uint16_t  uint16;   ///< Unsigned 16-bit integer.
    typedef uint32_t  uint32;   ///< Unsigned 32-bit integer.
    typedef uint64_t  uint64;   ///< Unsigned 64-bit integer.


    constexpr uint32 InvalidVidPnSourceId = ~0u; ///< In cases where PAL cannot abstract a Windows VidPnSourceId, this
    ///  represents an invalid value. (Note: zero is a valid value.)

    constexpr uint32 MaxVertexBuffers = 32;  ///< Maximum number of vertex buffers per pipeline.
    constexpr uint32 MaxColorTargets = 8;   ///< Maximum number of color targets.
    constexpr uint32 MaxStreamOutTargets = 4;   ///< Maximum number of stream output target buffers.
    constexpr uint32 MaxDescriptorSets = 2;   ///< Maximum number of descriptor sets.
    constexpr uint32 MaxMsaaRasterizerSamples = 16;  ///< Maximum number of MSAA samples supported by the rasterizer.
    constexpr uint32 MaxAvailableEngines = 12;  ///< Maximum number of engines for a particular engine type.
    constexpr uint32 MaxNumPlanes = 3;   ///< Maximum number of format planes.

    constexpr uint64 InternalApiPsoHash = UINT64_MAX;  ///< Default Hash for PAL internal pipelines.

    typedef enum BlendOp {
        BLEND_ZERO = 0x00000000,
        BLEND_ONE = 0x00000001,
        BLEND_SRC_COLOR = 0x00000002,
        BLEND_ONE_MINUS_SRC_COLOR = 0x00000003,
        BLEND_SRC_ALPHA = 0x00000004,
        BLEND_ONE_MINUS_SRC_ALPHA = 0x00000005,
        BLEND_DST_ALPHA = 0x00000006,
        BLEND_ONE_MINUS_DST_ALPHA = 0x00000007,
        BLEND_DST_COLOR = 0x00000008,
        BLEND_ONE_MINUS_DST_COLOR = 0x00000009,
        BLEND_SRC_ALPHA_SATURATE = 0x0000000a,
        BLEND_BOTH_SRC_ALPHA__GFX09_10 = 0x0000000b,
        BLEND_BOTH_INV_SRC_ALPHA__GFX09_10 = 0x0000000c,
        BLEND_CONSTANT_COLOR__GFX09_10 = 0x0000000d,
        BLEND_ONE_MINUS_CONSTANT_COLOR__GFX09_10 = 0x0000000e,
        BLEND_SRC1_COLOR__GFX09_10 = 0x0000000f,
        BLEND_INV_SRC1_COLOR__GFX09_10 = 0x00000010,
        BLEND_SRC1_ALPHA__GFX09_10 = 0x00000011,
        BLEND_INV_SRC1_ALPHA__GFX09_10 = 0x00000012,
        BLEND_CONSTANT_ALPHA__GFX09_10 = 0x00000013,
        BLEND_ONE_MINUS_CONSTANT_ALPHA__GFX09_10 = 0x00000014,
    } BlendOp;

    typedef enum SX_OPT_COMB_FCN {
        OPT_COMB_NONE = 0x00000000,
        OPT_COMB_ADD = 0x00000001,
        OPT_COMB_SUBTRACT = 0x00000002,
        OPT_COMB_MIN = 0x00000003,
        OPT_COMB_MAX = 0x00000004,
        OPT_COMB_REVSUBTRACT = 0x00000005,
        OPT_COMB_BLEND_DISABLED = 0x00000006,
        OPT_COMB_SAFE_ADD = 0x00000007,
    } SX_OPT_COMB_FCN;

    /// Comparison function determines how a pass/fail condition is determined between two values.  For depth/stencil
    /// comparison, the first value comes from source data and the second value comes from destination data.
    enum class CompareFunc : uint8
    {
        Never = 0x0,
        Less = 0x1,
        Equal = 0x2,
        LessEqual = 0x3,
        Greater = 0x4,
        NotEqual = 0x5,
        GreaterEqual = 0x6,
        _Always = 0x7,
        Count
    };
}
