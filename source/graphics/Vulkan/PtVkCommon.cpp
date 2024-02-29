#include <assert.h>
#include "PtVkCommon.h"

VkFormat GetVkRenderTargetFormatFromAMDFormat(uint8_t format, uint8_t numberType)
{
    switch (format)
    {
    case PtGfx::ColorFormat::COLOR_8:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UNORM:return VK_FORMAT_R8_UNORM;
        case PtGfx::NUMBER_SNORM:return VK_FORMAT_R8_SNORM;
        case PtGfx::NUMBER_UINT:return VK_FORMAT_R8_UINT;
        case PtGfx::NUMBER_SINT:return VK_FORMAT_R8_SINT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_8_8:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UNORM:   return VK_FORMAT_R8G8_UNORM;
        case PtGfx::NUMBER_SNORM:   return VK_FORMAT_R8G8_SNORM;
        case PtGfx::NUMBER_UINT:    return VK_FORMAT_R8G8_UINT;
        case PtGfx::NUMBER_SINT:    return VK_FORMAT_R8G8_SINT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_8_8_8_8:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UNORM:   return VK_FORMAT_R8G8B8A8_UNORM;
        case PtGfx::NUMBER_SRGB:    return VK_FORMAT_R8G8B8A8_SRGB;
        case PtGfx::NUMBER_SNORM:   return VK_FORMAT_R8G8B8A8_SNORM;
        case PtGfx::NUMBER_UINT:    return VK_FORMAT_R8G8B8A8_UINT;
        case PtGfx::NUMBER_SINT:    return VK_FORMAT_R8G8B8A8_SINT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_16:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UNORM:   return VK_FORMAT_R16_UNORM;
        case PtGfx::NUMBER_SNORM:   return VK_FORMAT_R16_SNORM;
        case PtGfx::NUMBER_UINT:    return VK_FORMAT_R16_UINT;
        case PtGfx::NUMBER_SINT:    return VK_FORMAT_R16_SINT;
        case PtGfx::NUMBER_FLOAT:   return VK_FORMAT_R16_SFLOAT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_16_16:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UNORM:   return VK_FORMAT_R16G16_UNORM;
        case PtGfx::NUMBER_SNORM:   return VK_FORMAT_R16G16_SNORM;
        case PtGfx::NUMBER_UINT:    return VK_FORMAT_R16G16_UINT;
        case PtGfx::NUMBER_SINT:    return VK_FORMAT_R16G16_SINT;
        case PtGfx::NUMBER_FLOAT:   return VK_FORMAT_R16G16_SFLOAT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_16_16_16_16:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UNORM:   return VK_FORMAT_R16G16B16A16_UNORM;
        case PtGfx::NUMBER_SNORM:   return VK_FORMAT_R16G16B16A16_SNORM;
        case PtGfx::NUMBER_UINT:    return VK_FORMAT_R16G16B16A16_UINT;
        case PtGfx::NUMBER_SINT:    return VK_FORMAT_R16G16B16A16_SINT;
        case PtGfx::NUMBER_FLOAT:   return VK_FORMAT_R16G16B16A16_SFLOAT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_32:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UINT:    return VK_FORMAT_R32_UINT;
        case PtGfx::NUMBER_SINT:    return VK_FORMAT_R32_SINT;
        case PtGfx::NUMBER_FLOAT:   return VK_FORMAT_R32_SFLOAT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_32_32:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UINT:    return VK_FORMAT_R32G32_UINT;
        case PtGfx::NUMBER_SINT:    return VK_FORMAT_R32G32_SINT;
        case PtGfx::NUMBER_FLOAT:   return VK_FORMAT_R32G32_SFLOAT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_32_32_32_32:
    {
        switch (numberType)
        {
        case PtGfx::NUMBER_UINT:    return VK_FORMAT_R32G32B32A32_UINT;
        case PtGfx::NUMBER_SINT:    return VK_FORMAT_R32G32B32A32_SINT;
        case PtGfx::NUMBER_FLOAT:   return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        break;
    }
    case PtGfx::ColorFormat::COLOR_2_10_10_10:
        switch (numberType)
        {
        case PtGfx::NUMBER_UNORM:return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
        }
    }
    return VkFormat();
}

VkFormat GetVkBufferFormatFromAMDFormat(PtGfx::BUF_DATA_FORMAT dataFormat, PtGfx::BUF_NUM_FORMAT numFormat)
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

VkFormat GetVkImgFormatFromAMDFormat(PtGfx::IMG_DATA_FORMAT imgFormat, PtGfx::IMG_NUM_FORMAT numFormat)
{
    switch (numFormat)
    {
    case PtGfx::IMG_NUM_FORMAT_UNORM:
    {
        switch (imgFormat)
        {
        case PtGfx::IMG_DATA_FORMAT_8_8_8_8:return VK_FORMAT_R8G8B8A8_UNORM;
        default:assert(false);
        }
    case PtGfx::IMG_NUM_FORMAT_SNORM:
        switch (imgFormat)
        {
        case PtGfx::IMG_DATA_FORMAT_8_8_8_8:return VK_FORMAT_R8G8B8A8_SNORM;
        default:assert(false);
        }
    case PtGfx::IMG_NUM_FORMAT_SRGB:
        switch (imgFormat)
        {
        case PtGfx::IMG_DATA_FORMAT_8_8_8_8:return VK_FORMAT_R8G8B8A8_SRGB;
        default:assert(false);
        }
    default:
        assert(false);
        break;
    };
    return VkFormat();
    };
}

VkCompareOp GetVkCmpFuncFromAMD(Pal::CompareFunc cmpFunc)
{
    return VkCompareOp(cmpFunc);
}
