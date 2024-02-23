#pragma once
#include "graphics\AMD\PtChip.h"
#include "graphics\AMD\pal\core\pal.h"
#include "vulkan\vulkan.h"

#define VULKAN_VARIFY(x)										\
    {															\
        VkResult Result = (x);									\
        if(Result != VkResult::VK_SUCCESS) { __debugbreak(); }	\
    }


VkFormat GetVkRenderTargetFormatFromAMDFormat(uint8_t format, uint8_t numberType);
VkFormat GetVkFormatFromAMDFormat(PtGfx::BUF_DATA_FORMAT dataFormat, PtGfx::BUF_NUM_FORMAT numFormat);
VkCompareOp GetVkCmpFuncFromAMD(Pal::CompareFunc cmpFunc);

