#include "pset_libkernel_impl.h"
#include "pset_kernel_error.h"

#include <thread>
#include <chrono>

int PSET_SYSV_ABI Pset_sceKernelWaitEqueue(SSceKernelEqueue* sceKernalEqueue, SceKernelEvent* sceKernelEvent, int num, int* out, uint32_t* sceKernelUseconds)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceKernelWaitEqueue");

	SceKernelEvent debugEvent = *sceKernelEvent;
	sceKernelEvent->flags = 1;
	sceKernelEvent->filter = 1;
	sceKernelEvent->fflags = 1;
	if (out)
	{
		*out  = *out + 10000;
	}

	
	std::this_thread::sleep_for(std::chrono::microseconds(10));
	
	return 0;
}