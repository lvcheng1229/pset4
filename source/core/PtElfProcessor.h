#pragma once
#include <vector>

#include "PtNativeModule.h"

struct SElfProcessorCfg
{
	
};

class CPtElfProcessor
{
public:
	CPtElfProcessor(SElfProcessorCfg elfProcessorCfg);

	void SetProcessContext(CPtNativeModule* outputModule, std::string elfPath);
	void LoadRawData(std::vector<uint8_t>& outRawData);
	void MapSelfToElf(std::vector<uint8_t>& selfRawData);
	void ParseProgramHeaders();
	void ParseDynamicSegment();
	void MapImageToMemory();
	void ParseAndExportNativeSymbol();
	void AddModuleDependencies();
private:
	size_t CalculateTotalLoadableSize();
	void MapSegment(Elf64_Phdr const& hdr);
	void RelocateNativeLocalSymbol(Elf64_Rela* pReallocateTable, uint32_t relaCount);

	SElfProcessorCfg m_elfProcessorCfg;

	std::string m_elfPath;
	CPtNativeModule* m_module;
};
CPtElfProcessor* GetElfProcessor();