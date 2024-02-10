#include "pset_libkernel_memory.h"

size_t PSET_SYSV_ABI Pset_sceKernelGetDirectMemorySize(void)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceKernelGetDirectMemorySize");
	return SCE_KERNEL_MAIN_DMEM_SIZE;
}