#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "system/elf.h"
#include "system/SystemTypes.h"

struct SModuleInfo
{
	void* pEntryPoint;

	SMemoryChrunk m_mappedMemory;

	SMemoryChrunk m_sceStrTable;
	SMemoryChrunk m_pSceDynamicLib;
	SMemoryChrunk m_relocationTable;
	SMemoryChrunk m_relocationPltTable;
	SMemoryChrunk m_symbleTable;

};

struct SModuleValue
{
	union {
		uint64_t value;
		struct
		{
			uint32_t name_offset;
			uint8_t version_minor;
			uint8_t version_major;
			uint16_t m_id;
		};
	};
};

struct SPsetModule
{
	std::string m_moduleName;

};

struct SLibraryValue
{
	union {
		uint64_t m_value;
		struct
		{
			uint32_t m_name_offset;
			uint16_t m_version;
			uint16_t m_id;
		};
	};
};

struct SPsetLibrary
{
	std::string m_libraryName;
};

class CPsetModule
{
	friend class CElfProcessor;
public:
	void* GetEntryPoint()const;

	inline SModuleInfo& GetModuleInfo()
	{
		return m_moduleInfo;
	}

	inline std::unordered_map<uint16_t, SPsetLibrary>& GetId2LibMap()
	{
		return m_id2LibraryNameMap;
	}

	inline std::unordered_map<uint16_t, SPsetModule>& GetId2ModuleMap()
	{
		return m_id2ModuleNameMap;
	}
private:
	SModuleInfo m_moduleInfo;
	

	Elf64_Ehdr m_elf64Ehdr;
	std::vector<Elf64_Phdr> m_aSegmentHeaders;

	std::unordered_map<uint16_t, SPsetLibrary> m_id2LibraryNameMap;
	std::unordered_map<uint16_t, SPsetModule> m_id2ModuleNameMap;
};