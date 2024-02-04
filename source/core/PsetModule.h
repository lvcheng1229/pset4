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

struct SPsetModule
{
	std::string strName;
	//union {
	//	uint64_t value;
	//	struct
	//	{
	//		uint32_t name_offset;
	//		uint8_t version_minor;
	//		uint8_t version_major;
	//		uint16_t id;
	//	};
	//};
};

struct SPsetLibraryInfo
{
	union {
		uint64_t value;
		struct
		{
			uint32_t name_offset;
			uint16_t version;
			uint16_t id;
		};
	};
};

struct SPsetLibrary
{
	std::string m_libraryName;
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

	std::unordered_map<uint16_t, SPsetLibrary>& GetId2LibMap()
	{
		return m_id2LibraryNameMap;
	}
private:
	SModuleInfo m_moduleInfo;
	std::vector<std::string>m_aNeededFiles;

	Elf64_Ehdr m_elf64Ehdr;
	std::vector<Elf64_Phdr> m_aSegmentHeaders;

	std::unordered_map<uint16_t, SPsetLibrary> m_id2LibraryNameMap;
};