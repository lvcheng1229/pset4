#include "pset_libkernel_memory.h"

int32_t CLibKernelMemoryManager::AllocDirectMemory(int64_t searchStart, int64_t searchEnd, uint64_t length, uint64_t alignment, int memoryType, uint64_t* physicalAddrDest)
{
	if (!length || ((length % SCE_KERNEL_PAGE_SIZE) != 0))
	{
		return SCE_KERNEL_ERROR_EINVAL;
	}

	if (alignment != 0 && ((alignment % SCE_KERNEL_PAGE_SIZE) != 0))
	{
		return SCE_KERNEL_ERROR_EINVAL;
	}

	*physicalAddrDest = SCE_KERNEL_APP_MAP_AREA_START_ADDR;
	return PSET_OK;
}

int32_t CLibKernelMemoryManager::MapDirectMemory(void** virtualAddrDest, uint64_t length, int prot, int flags, uint64_t physicalAddr, uint64_t alignment)
{
	if (!virtualAddrDest) { return SCE_KERNEL_ERROR_EINVAL; }
	if (!length || ((length % SCE_KERNEL_PAGE_SIZE) != 0)) { return SCE_KERNEL_ERROR_EINVAL; }
	if ((physicalAddr % SCE_KERNEL_PAGE_SIZE) != 0) { return SCE_KERNEL_ERROR_EINVAL; }
	if (alignment != 0 && ((alignment % SCE_KERNEL_PAGE_SIZE) != 0)) { return SCE_KERNEL_ERROR_EINVAL; }
	if ((flags & SCE_KERNEL_MAP_FIXED) && (((uint64_t)virtualAddrDest % SCE_KERNEL_PAGE_SIZE) != 0)) { return SCE_KERNEL_ERROR_EINVAL; }
	void* resAddress = AllocateImpl(*virtualAddrDest, length, prot, alignment);
	assert(resAddress != nullptr);
	if (resAddress == nullptr)
	{
		return SCE_KERNEL_ERROR_EINVAL;
	}
	*virtualAddrDest = resAddress;
	return PSET_OK;
}

uint32_t CLibKernelMemoryManager::GetProtectFlag(int prot)
{
	uint32_t protFalg = PAGE_NOACCESS;

	if ((prot & SCE_KERNEL_PROT_CPU_READ) || (prot & SCE_KERNEL_PROT_GPU_READ))
	{
		protFalg = PAGE_READONLY;
	}

	if ((prot & SCE_KERNEL_PROT_CPU_WRITE) || (prot & SCE_KERNEL_PROT_GPU_WRITE))
	{
		protFalg = PAGE_READWRITE;
	}

	if (prot & SCE_KERNEL_PROT_CPU_EXEC)
	{
		protFalg = PAGE_EXECUTE_READ;
	}

	return protFalg;
}

void* CLibKernelMemoryManager::AllocateImpl(void* addrIn, uint64_t length, int prot, uint64_t alignment)
{
	void* addrOut = nullptr;

	uint32_t protectFlag = GetProtectFlag(prot);
	if (alignment == 0) { alignment = SCE_KERNEL_PAGE_SIZE; };

	size_t searchAddr = SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR;
	if (addrIn != nullptr)
	{
		searchAddr = reinterpret_cast<size_t>(addrIn);
	}
	
	while (searchAddr < SCE_KERNEL_APP_MAP_AREA_END_ADDR)
	{
		MEMORY_BASIC_INFORMATION mbi = {};
		if (VirtualQuery(reinterpret_cast<void*>(searchAddr), &mbi, sizeof(mbi)) == 0) { assert(false); }

		if (mbi.State != MEM_FREE)
		{
			searchAddr = reinterpret_cast<size_t>(mbi.BaseAddress) + mbi.RegionSize;
			continue;
		}

		size_t regionAddress = reinterpret_cast<size_t>(mbi.BaseAddress);
		size_t regionSize = mbi.RegionSize;
		size_t alignPad = regionAddress & (alignment - 1);
		if (alignPad)
		{
			size_t offset = (alignment - alignPad);
			regionAddress += offset;
			if (regionSize > offset)
			{
				regionSize -= offset;
			}
			else
			{
				regionSize = 0;
			}
		}

		if (regionSize < length)
		{
			searchAddr = reinterpret_cast<size_t>(mbi.BaseAddress) + mbi.RegionSize;
			continue;
		}

		void* retAddress = VirtualAlloc(reinterpret_cast<void*>(regionAddress), length, MEM_RESERVE | MEM_COMMIT, protectFlag);
		if (!retAddress)
		{
			searchAddr = reinterpret_cast<size_t>(mbi.BaseAddress) + mbi.RegionSize;
			continue;
		}

		addrOut = retAddress;
		if (addrOut == nullptr) 
		{ 
			assert(false); 
		}
		searchAddr = reinterpret_cast<size_t>(mbi.BaseAddress) + mbi.RegionSize;
	}

	return addrOut;
}

static CLibKernelMemoryManager gLibKernelMemoryManager;

size_t PSET_SYSV_ABI Pset_sceKernelGetDirectMemorySize(void)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceKernelGetDirectMemorySize");
	return SCE_KERNEL_MAIN_DMEM_SIZE;
}

int PSET_SYSV_ABI Pset_sceKernelAllocateDirectMemory(int64_t searchStart, int64_t searchEnd, uint64_t length, uint64_t alignment, int memoryType, uint64_t* physicalAddrDest)
{
 	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceKernelAllocateDirectMemory");
	gLibKernelMemoryManager.AllocDirectMemory(searchStart, searchEnd, length, alignment, memoryType, physicalAddrDest);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceKernelMapDirectMemory(void** virtualAddrDest, uint64_t length, int prot, int flags, int64_t physicalAddr, uint64_t alignment)
{
	gLibKernelMemoryManager.MapDirectMemory(virtualAddrDest, length, prot, flags, physicalAddr, alignment);
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceKernelMapDirectMemory");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceKernelMapDirectMemory2(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceKernelMapDirectMemory2");
	return PSET_OK;
}

