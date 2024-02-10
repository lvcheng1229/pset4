#pragma once
#include "overridemodule\PsetLibraryCommon.h" 

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


size_t PSET_SYSV_ABI Pset_sceKernelGetDirectMemorySize(void);