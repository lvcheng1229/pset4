#pragma once
#include <vector>
#include "../system/elf.h"
#include "../system/SystemTypes.h"

struct SModuleInfo
{
	void* pEntryPoint;

	SMemoryChrunk m_mappedMemory;
};

class CPsetModule
{
	friend class CPsetElf;
public:
	void* GetEntryPoint()const;
private:
	SModuleInfo m_moduleInfo;


	Elf64_Ehdr m_elf64Ehdr;
	std::vector<Elf64_Phdr> m_aSegmentHeaders;
};