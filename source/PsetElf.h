#pragma once
#include <string>
#include <vector>

#include "system/elf.h"

#include "system/SystemTypes.h"
#include "system/Module.h"

#include "core/PsetTLS.h"

class CPsetModule;
class CPsetElf
{
public:
	~CPsetElf();
	void LoadFromFile(const std::string& elfPath, CPsetModule* psetModule);

	void MapImageIntoMemory();
	void PrepareProgramHeader();
	void PrepreDynamicSegments();

	std::vector<std::string>& GetNeededFiles();
private:

	void PrepareTables(Elf64_Dyn& elf64Dyn);
	void ParseSingleDynamicEntry(Elf64_Dyn& elf64Dyn);

	void LoadSymbolExport();
	void PatchTls();

	void MapCodeInit();
	void ModuleStart();

	bool mapCodeSegment(Elf64_Phdr const& hdr);

	size_t CalculateTotalLoadableSize();
	bool IsSegmentLoadable(Elf64_Phdr const& hdr);
private:
	SHeaderSelf m_headerSelf;
	std::string m_fileName;
	SMemoryChrunk m_elf;
	SSceKernelModuleInfo m_moduleInfo;

	//TODO: module info abstraction
	
	//PrepareProgramHeader:BEGIN
	void* m_pProcParam;
	void* m_pModuleParam;
	void* m_pInterProgram;

	Elf64_Dyn* m_pDynamicEntry;
	uint32_t m_nDynamicEntryCount = 0;
	
	

	STLSBlock m_tlsBlock;

	SMemoryChrunk m_pEHFrameHeader;

	struct SEhFrameHeader
	{
		uint64_t m_address;
		uint64_t m_size;
	};
	SEhFrameHeader m_ehFrameHeader;
	//PrepareProgramHeader:END

	//PrepareTables:BEGIN
	//void* m_pInitProc; // pointer to .init_proc()
	//PrepareTables:END

	CPsetModule* m_moduleToLoad;
	
};