#include <assert.h>
#include "PtVkCommon.h"

VkFormat GetVkFormatFromAMDFormat(PtGfx::BUF_DATA_FORMAT dataFormat, PtGfx::BUF_NUM_FORMAT numFormat)
{
    switch (numFormat)
    {
    case PtGfx::BUF_NUM_FORMAT_UNORM:
    {
        switch (dataFormat)
        {
        case PtGfx::BUF_DATA_FORMAT_8:return VK_FORMAT_R8_UNORM;
        case PtGfx::BUF_DATA_FORMAT_16:return VK_FORMAT_R16_UNORM;
        case PtGfx::BUF_DATA_FORMAT_8_8:return VK_FORMAT_R8G8_UNORM;
        case PtGfx::BUF_DATA_FORMAT_32:assert(false);
        case PtGfx::BUF_DATA_FORMAT_16_16:return VK_FORMAT_R16G16_UNORM;
        case PtGfx::BUF_DATA_FORMAT_10_11_11:assert(false);
        case PtGfx::BUF_DATA_FORMAT_11_11_10 :assert(false);
        case PtGfx::BUF_DATA_FORMAT_10_10_10_2:assert(false);
        case PtGfx::BUF_DATA_FORMAT_2_10_10_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_8_8_8_8:return VK_FORMAT_R8G8B8A8_UNORM;
        case PtGfx::BUF_DATA_FORMAT_32_32:assert(false);
        case PtGfx::BUF_DATA_FORMAT_16_16_16_16:return VK_FORMAT_R16G16B16A16_UNORM;
        case PtGfx::BUF_DATA_FORMAT_32_32_32:assert(false);
        case PtGfx::BUF_DATA_FORMAT_32_32_32_32:assert(false);
        }
        break;
    }
    case PtGfx::BUF_NUM_FORMAT_SNORM:
    {
        switch (dataFormat)
        {
        case PtGfx::BUF_DATA_FORMAT_8:return VK_FORMAT_R8_SNORM;
        case PtGfx::BUF_DATA_FORMAT_16:return VK_FORMAT_R16_SNORM;
        case PtGfx::BUF_DATA_FORMAT_8_8:return VK_FORMAT_R8G8_SNORM;
        case PtGfx::BUF_DATA_FORMAT_32:assert(false);
        case PtGfx::BUF_DATA_FORMAT_16_16:return VK_FORMAT_R16G16_SNORM;
        case PtGfx::BUF_DATA_FORMAT_10_11_11:assert(false);
        case PtGfx::BUF_DATA_FORMAT_11_11_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_10_10_10_2:assert(false);
        case PtGfx::BUF_DATA_FORMAT_2_10_10_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_8_8_8_8:return VK_FORMAT_R8G8B8A8_SNORM;
        case PtGfx::BUF_DATA_FORMAT_32_32:assert(false);
        case PtGfx::BUF_DATA_FORMAT_16_16_16_16:return VK_FORMAT_R16G16B16A16_SNORM;
        case PtGfx::BUF_DATA_FORMAT_32_32_32:assert(false);
        case PtGfx::BUF_DATA_FORMAT_32_32_32_32:assert(false);
        }
        break;
    }
    case PtGfx::BUF_NUM_FORMAT_UINT:
    {
        switch (dataFormat)
        {
        case PtGfx::BUF_DATA_FORMAT_8:return VK_FORMAT_R8_UINT;
        case PtGfx::BUF_DATA_FORMAT_16:return VK_FORMAT_R16_UINT;
        case PtGfx::BUF_DATA_FORMAT_8_8:return VK_FORMAT_R8G8_UINT;
        case PtGfx::BUF_DATA_FORMAT_32:return VK_FORMAT_R32_UINT;
        case PtGfx::BUF_DATA_FORMAT_16_16:return VK_FORMAT_R16G16_UINT;
        case PtGfx::BUF_DATA_FORMAT_10_11_11:assert(false);
        case PtGfx::BUF_DATA_FORMAT_11_11_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_10_10_10_2:assert(false);
        case PtGfx::BUF_DATA_FORMAT_2_10_10_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_8_8_8_8:return VK_FORMAT_R8G8B8A8_UINT;
        case PtGfx::BUF_DATA_FORMAT_32_32:return VK_FORMAT_R32G32_UINT;
        case PtGfx::BUF_DATA_FORMAT_16_16_16_16:return VK_FORMAT_R16G16B16A16_UINT;
        case PtGfx::BUF_DATA_FORMAT_32_32_32:return VK_FORMAT_R32G32B32_UINT;
        case PtGfx::BUF_DATA_FORMAT_32_32_32_32:return VK_FORMAT_R32G32B32A32_UINT;
        default:assert(false);
        }
        break;
    }
    case PtGfx::BUF_NUM_FORMAT_SINT:
    {
        switch (dataFormat)
        {
        case PtGfx::BUF_DATA_FORMAT_8:return VK_FORMAT_R8_SINT;
        case PtGfx::BUF_DATA_FORMAT_16:return VK_FORMAT_R16_SINT;
        case PtGfx::BUF_DATA_FORMAT_8_8:return VK_FORMAT_R8G8_SINT;
        case PtGfx::BUF_DATA_FORMAT_32:return VK_FORMAT_R32_SINT;
        case PtGfx::BUF_DATA_FORMAT_16_16:return VK_FORMAT_R16G16_SINT;
        case PtGfx::BUF_DATA_FORMAT_10_11_11:assert(false);
        case PtGfx::BUF_DATA_FORMAT_11_11_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_10_10_10_2:assert(false);
        case PtGfx::BUF_DATA_FORMAT_2_10_10_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_8_8_8_8:return VK_FORMAT_R8G8B8A8_SINT;
        case PtGfx::BUF_DATA_FORMAT_32_32:return VK_FORMAT_R32G32_SINT;
        case PtGfx::BUF_DATA_FORMAT_16_16_16_16:return VK_FORMAT_R16G16B16A16_SINT;
        case PtGfx::BUF_DATA_FORMAT_32_32_32:return VK_FORMAT_R32G32B32_SINT;
        case PtGfx::BUF_DATA_FORMAT_32_32_32_32:return VK_FORMAT_R32G32B32A32_SINT;
        default:assert(false);
        }
        break;
    }
    case PtGfx::BUF_NUM_FORMAT_FLOAT:
    {
        switch (dataFormat)
        {
        case PtGfx::BUF_DATA_FORMAT_8:assert(false);
        case PtGfx::BUF_DATA_FORMAT_16:return VK_FORMAT_R16_SFLOAT;
        case PtGfx::BUF_DATA_FORMAT_8_8:assert(false);
        case PtGfx::BUF_DATA_FORMAT_32:return VK_FORMAT_R32_SFLOAT;
        case PtGfx::BUF_DATA_FORMAT_16_16:return VK_FORMAT_R16G16_SFLOAT;
        case PtGfx::BUF_DATA_FORMAT_10_11_11:assert(false);
        case PtGfx::BUF_DATA_FORMAT_11_11_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_10_10_10_2:assert(false);
        case PtGfx::BUF_DATA_FORMAT_2_10_10_10:assert(false);
        case PtGfx::BUF_DATA_FORMAT_8_8_8_8:assert(false);
        case PtGfx::BUF_DATA_FORMAT_32_32:return VK_FORMAT_R32G32_SFLOAT;
        case PtGfx::BUF_DATA_FORMAT_16_16_16_16:return VK_FORMAT_R16G16B16A16_SFLOAT;
        case PtGfx::BUF_DATA_FORMAT_32_32_32:return VK_FORMAT_R32G32B32_SFLOAT;
        case PtGfx::BUF_DATA_FORMAT_32_32_32_32:return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:assert(false);
        }
        break;
    }
    default:
        assert(false);
        break;
    }
    return VkFormat();
}
