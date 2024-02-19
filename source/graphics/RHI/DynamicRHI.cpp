#include "DynamicRHI.h"
#include "graphics\Vulkan\PtVulkanDynamicRHI.h"

CDynamicRHI* gPlatformRHI = nullptr;

void InitDynamicRHI(void* windowHandle, bool useVulkan, bool useDx12)
{
	if (useVulkan)
	{
		gPlatformRHI = new CVulkanDynamicRHI();
	}

	gPlatformRHI->Init(windowHandle);
}
