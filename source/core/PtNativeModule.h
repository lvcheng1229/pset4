#pragma once
#include <unordered_map>
#include <string>

#include "system\elf.h"
#include "system\systemTypes.h"
#include "Log.h"

struct SPtModuleInfo
{
	void* m_pEntryPoint;

	SMemoryChrunk m_mappedMemory;

	// ParseProgramHeaders
	void* m_pProcParam;
	void* m_pModuleParam;
	void* m_pInterProgram;
	void* m_pDynamicEntry;
	uint32_t m_nDynamicEntryCount;
	SMemoryChrunk m_pSceDynamicLib;

	//ParseDynamicSegment
	SMemoryChrunk m_sceStrTable;
	SMemoryChrunk m_relocationTable;
	SMemoryChrunk m_relocationPltTable;
	SMemoryChrunk m_symbleTable;
};

struct SSPtModuleValue
{
	union {
		uint64_t value;
		struct
		{
			uint32_t name_offset;
			uint8_t version_minor;
			uint8_t version_major;
			uint16_t id;
		};
	};
};

struct SPtLibraryValue
{
	union {
		uint64_t m_value;
		struct
		{
			uint32_t name_offset;
			uint16_t version;
			uint16_t id;
		};
	};
};

class CPtNativeModule
{
	friend class CPtModuleLoader;
	friend class CPtElfProcessor;
public:
	void GetLibAndModuleName(uint16_t libId, uint16_t moduleId, std::string& outlibName, std::string& outModulebName);
	inline SPtModuleInfo& GetModuleInfo() { return m_moduleInfo; }
private:
	std::unordered_map<uint16_t, std::string> m_id2LibraryNameMap;
	std::unordered_map<uint16_t, std::string> m_id2ModuleNameMap;


	Elf64_Ehdr m_self64Ehdr;
	std::vector<Elf64_Phdr> m_aSegmentHeaders;
	std::vector<uint8_t> m_elfData;

	std::vector<std::string> m_neededFiles;

	std::string m_fileName;
	SPtModuleInfo m_moduleInfo;
};