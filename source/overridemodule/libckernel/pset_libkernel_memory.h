#pragma once
#include "overridemodule\PsetLibraryCommon.h" 
#include "pset_kernel_error.h"

//https://github.com/red-prig/fpPS4
/*
Flexible memory : elf sections, stack, calloc, cpu only
Direct memory : phisical mapped, cpu / gpu mem
Pooled memory : section of direct memory, 64 KiB blocks,
The application program can use a total of 5184 MiB(5824 MiB in NEO mode) physical memory.
If there is no specification, 448 MiB will be assigned as flexible memory.
Physical Address Space and Direct Memory Areas is guaranteed to be aligned to a 2 MiB boundary.
Unmapped area : 0x0000 0000 0000 - 0x0000 0040 0000 Size : 0x0000 0040 0000 (4MB)
System managed area : 0x0000 0040 0000 - 0x0007 FFFF C000 Size : 0x0007 FFBF C000(31GB)
System reserved area : 0x0007 FFFF C000 - 0x0010 0000 0000 Size : 0x0008 0000 4000 (32GB)
User area : 0x0010 0000 0000 - 0x00FC 0000 0000 Size : 0x00EC 0000 0000 (944GB)
System reserved area : 0x00FC 0000 0000 - 0x00FF FFFF FFFF Size : 0x0003 FFFF FFFF(15GB)
*/

#define SCE_KERNEL_MAIN_DMEM_SIZE 0x180000000 //6GB

#define SCE_KERNEL_APP_MAP_AREA_START_ADDR 0x1000000000ULL
#define SCE_KERNEL_APP_MAP_AREA_END_ADDR   0xfc00000000ULL
#define SCE_KERNEL_APP_MAP_AREA_SIZE \
	(SCE_KERNEL_APP_MAP_AREA_END_ADDR - SCE_KERNEL_APP_MAP_AREA_START_ADDR)

#define SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR 0x000000400000ULL
#define SCE_KERNEL_SYS_MANAGE_AREA_END_ADDR   0x0007ffffc000ULL
#define SCE_KERNEL_SYS_MANAGE_AREA_SIZE \
	(SCE_KERNEL_SYS_MANAGE_AREA_END_ADDR - SCE_KERNEL_SYS_MANAGE_AREA_START_ADDR)

#define SCE_KERNEL_PAGE_SIZE 0x4000ULL
#define SCE_KERNEL_MAP_FIXED 0x0010ULL

#define	SCE_KERNEL_PROT_CPU_READ  0x01
#define	SCE_KERNEL_PROT_CPU_RW	  0x02
#define	SCE_KERNEL_PROT_CPU_WRITE 0x02
#define	SCE_KERNEL_PROT_CPU_EXEC  0x04
#define	SCE_KERNEL_PROT_CPU_ALL	  0x07

#define SCE_KERNEL_PROT_GPU_READ  0x10
#define SCE_KERNEL_PROT_GPU_WRITE 0x20
#define SCE_KERNEL_PROT_GPU_RW	  0x30
#define SCE_KERNEL_PROT_GPU_ALL	  0x30

class CLibKernelMemoryManager
{
public:
	int32_t AllocDirectMemory(int64_t searchStart, int64_t searchEnd, uint64_t length, uint64_t alignment, int memoryType, uint64_t* physicalAddrDest);
	int32_t MapDirectMemory(void** virtualAddrDest, uint64_t length, int prot, int flags, uint64_t physicalAddr, uint64_t alignment);
private:
	void* AllocateImpl(void* addrIn, uint64_t length, int prot, uint64_t alignment);
	uint32_t GetProtectFlag(int prot);
};

size_t PSET_SYSV_ABI Pset_sceKernelGetDirectMemorySize(void);
int PSET_SYSV_ABI Pset_sceKernelAllocateDirectMemory(int64_t searchStart, int64_t searchEnd, uint64_t length, uint64_t alignment, int memoryType, uint64_t* physicalAddrDest);
int PSET_SYSV_ABI Pset_sceKernelMapDirectMemory(void** virtualAddrDest, uint64_t length, int prot, int flags, int64_t physicalAddr, uint64_t alignment);
int PSET_SYSV_ABI Pset_sceKernelMapDirectMemory2(void);