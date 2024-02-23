#include "DynamicRHI.h"

#include "graphics\RHI\RHICommandList.h"
#include "graphics\Vulkan\PtVulkanDynamicRHI.h"

CDynamicRHI* gPlatformRHI = nullptr;
CRHICommnadList gRHICommandList;

void InitDynamicRHI(void* windowHandle, bool useVulkan, bool useDx12)
{
	if (useVulkan)
	{
		gPlatformRHI = new CVulkanDynamicRHI();
	}

	gPlatformRHI->Init(windowHandle);
}
