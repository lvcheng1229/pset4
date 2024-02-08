#pragma once
#include <string>
#include <vector>

#include "system/elf.h"

#include "system/SystemTypes.h"

#include "core/PsetTLS.h"

class CPsetModule;
class CElfProcessor
{
public:
	~CElfProcessor();
	void LoadFromFile(const std::string& elfPath, CPsetModule* psetModule, bool bEBootModule = false);

	void MapImageIntoMemory();
	void PrepareProgramHeader();
	void PrepreDynamicSegments();

	std::vector<std::string>& GetNeededFiles();
private:

	void PrepareTables(Elf64_Dyn& elf64Dyn);
	void ParseSingleDynamicEntry(Elf64_Dyn& elf64Dyn);

	void MapCodeInit();

	bool MapSegment(Elf64_Phdr const& hdr);

	size_t CalculateTotalLoadableSize();
	bool IsSegmentLoadable(Elf64_Phdr const& hdr);
private:
	std::vector<std::string>m_aNeededFiles;

	SHeaderSelf m_headerSelf;
	std::string m_fileName;
	SMemoryChrunk m_elf;
	
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

	CPsetModule* m_moduleToLoad;
	bool bEbootModule;
};