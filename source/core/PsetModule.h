#pragma once
#include <vector>
#include <string>

#include "../system/elf.h"
#include "../system/SystemTypes.h"

struct SModuleInfo
{
	void* pEntryPoint;

	SMemoryChrunk m_sceStrTable;
	SMemoryChrunk m_pSceDynamicLib;
	SMemoryChrunk m_mappedCodeMemory;
	SMemoryChrunk m_relocationTable;
	SMemoryChrunk m_symbleTable;
};

class CPsetModule
{
	friend class CPsetElf;
public:
	void* GetEntryPoint()const;

	inline SModuleInfo& GetModuleInfo()
	{
		return m_moduleInfo;
	}
private:
	SModuleInfo m_moduleInfo;
	std::vector<std::string>m_aNeededFiles;

	Elf64_Ehdr m_elf64Ehdr;
	std::vector<Elf64_Phdr> m_aSegmentHeaders;
};