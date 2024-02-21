#pragma once
#include "graphics\AMD\PtChip.h"
#include "vulkan\vulkan.h"

#define VULKAN_VARIFY(x)										\
    {															\
        VkResult Result = (x);									\
        if(Result != VkResult::VK_SUCCESS) { __debugbreak(); }	\
    }


VkFormat GetVkFormatFromAMDFormat(PtGfx::BUF_DATA_FORMAT dataFormat, PtGfx::BUF_NUM_FORMAT numFormat);

