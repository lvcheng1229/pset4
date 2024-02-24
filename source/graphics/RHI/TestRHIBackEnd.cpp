#include "RHIDeviceCommand.h"

#include "graphics\Vulkan\PtVkDeviceCommand.h"

void TestRHIFunc(void* handle)
{
	auto* tt = new CPtVkDeviceCommand();
	tt->Init(handle);
	tt->TestMainLoop();
}