#include "system\elf.h"
#include "PtUtil.h"
#include "PtDynamicLinker.h"
#include "PtElfProcessor.h"
#include "PtModuleLoader.h"

CPtElfProcessor::CPtElfProcessor(SElfProcessorCfg elfProcessorCfg)
{
	m_elfProcessorCfg = elfProcessorCfg;
}

void CPtElfProcessor::SetProcessContext(CPtNativeModule* outputModule, std::string elfPath)
{
	m_module = outputModule;
	m_elfPath = elfPath;
}

void CPtElfProcessor::LoadRawData(std::vector<uint8_t>& outRawData)
{
	std::ifstream fin(m_elfPath, std::ifstream::in | std::ifstream::ate | std::ifstream::binary);
	PSET_EXIT_AND_LOG_IF(!fin.is_open(), "couldn't open app");

	size_t nFileSize = (size_t)fin.tellg();
	outRawData.resize(nFileSize);

	fin.seekg(0);
	fin.read((char*)outRawData.data(), nFileSize);
	fin.close();
}

static void ValidateElf(elf64_hdr* elf64)
{
	Elf64_Half elfType = elf64->e_type;

	PSET_EXIT_AND_LOG_IF(strncmp((const char*)elf64->e_ident, ELFMAG, 4), "elf file magic number mismatch");
	PSET_EXIT_AND_LOG_IF(elfType != ET_SCE_EXEC && elfType != ET_SCE_REPLAY_EXEC && elfType != ET_SCE_RELEXEC && elfType != ET_SCE_DYNEXEC && elfType != ET_SCE_DYNAMIC, "unsupported type");
	PSET_EXIT_AND_LOG_IF(elf64->e_machine != EM_X86_64, "unsupported machine");
	PSET_EXIT_AND_LOG_IF(elf64->e_phnum == 0, "the number of entries in the program header table is zero");
}

static uint32_t FlagsId(uint64_t flag)
{
	return (flag >> 20) & 0xFFF;
}

void CPtElfProcessor::MapSelfToElf(std::vector<uint8_t>& selfRawData)
{
	SHeaderSelf* pHeaderSelf = (SHeaderSelf*)selfRawData.data();

	if ((*((uint32_t*)pHeaderSelf->m_magic)) == uint32_t(ELFMAG))
	{
		PSET_EXIT_AND_LOG("unsupported format");
	}
	else if ((*(uint32_t*)(pHeaderSelf->m_magic)) == uint32_t(PSET_SELFMAG))
	{
		uint16_t numSegments = *(uint16_t*)(&pHeaderSelf->m_numberOfSegments);

		SSegStructureSelf* pSegStructureSelfs = (SSegStructureSelf*)((uint8_t*)pHeaderSelf + sizeof(SHeaderSelf));
		for (uint32_t index = 0; index < numSegments; index++)
		{
			SSegStructureSelf& pSegStructureSelf = pSegStructureSelfs[index];
			PSET_EXIT_AND_LOG_IF((pSegStructureSelf.m_flags == uint64_t(ESegFlags::SF_ENCR) || pSegStructureSelf.m_flags == uint64_t(ESegFlags::SF_DFLG)), "unsupport format");
		}

		elf64_hdr* elf64Header = (elf64_hdr*)(pSegStructureSelfs + numSegments);
		ValidateElf(elf64Header);

		m_module->m_self64Ehdr = *elf64Header;

		elf64_phdr* elf64ProgramHeader = (elf64_phdr*)(elf64Header + 1);

		m_module->m_aSegmentHeaders.resize(elf64Header->e_phnum);
		for (uint32_t index = 0; index < elf64Header->e_phnum; index++)
		{
			m_module->m_aSegmentHeaders[index] = elf64ProgramHeader[index];
		}

		uint32_t segmentsStart = std::numeric_limits<uint32_t>::max();
		uint32_t segmentsEnd = 0;

		for (uint32_t index = 0; index < elf64Header->e_phnum; index++)
		{
			uint32_t segmentStart = elf64ProgramHeader[index].p_offset;
			if (segmentStart > 0 && segmentStart < segmentsStart)
			{
				segmentsStart = segmentStart;
			}

			uint32_t segmentEnd = segmentStart + elf64ProgramHeader[index].p_filesz;
			if (segmentEnd > segmentsEnd)
			{
				segmentsEnd = segmentEnd;
			}
		}

		m_module->m_elfData.resize(segmentsEnd);
		memcpy(m_module->m_elfData.data(), elf64Header, segmentsStart);

		for (uint32_t index = 0; index < numSegments; index++)
		{
			if ((pSegStructureSelfs[index].m_flags & uint64_t(ESegFlags::SF_BFLG)) != 0)
			{
				uint32_t flagID = FlagsId(pSegStructureSelfs[index].m_flags);
				memcpy(m_module->m_elfData.data() + elf64ProgramHeader[flagID].p_offset, selfRawData.data() + pSegStructureSelfs[index].m_offsets, pSegStructureSelfs[index].m_encryptedCompressedSize);
			}
		}
	}
	else
	{
		PSET_EXIT_AND_LOG("unsupported format");
	}
}

void CPtElfProcessor::ParseProgramHeaders()
{
	SPtModuleInfo& moduleInfo = m_module->m_moduleInfo;
	elf64_hdr* pElfHdr = (elf64_hdr*)(m_module->m_elfData.data());
	elf64_phdr* pElfpHdr = (elf64_phdr*)(pElfHdr + 1);

	for (uint32_t index = 0; index < pElfHdr->e_phnum; index++)
	{
		switch (pElfpHdr[index].p_type)
		{
		case PT_NOTE:
		case PT_LOAD:
		case PT_SCE_RELRO:
		case PT_SCE_COMMENT:
			break;
		case PT_SCE_PROCPARAM:
			moduleInfo.m_nProcParamOffset = pElfpHdr[index].p_paddr;
			break;
		case PT_INTERP:
			moduleInfo.m_pInterProgram = m_module->m_elfData.data() + pElfpHdr[index].p_offset;
			break;
		case PT_DYNAMIC:
			moduleInfo.m_pDynamicEntry = m_module->m_elfData.data() + pElfpHdr[index].p_offset;
			moduleInfo.m_nDynamicEntryCount = pElfpHdr[index].p_filesz / sizeof(Elf64_Dyn);
			PSET_EXIT_AND_LOG_IF(moduleInfo.m_nDynamicEntryCount == 0, "empty dynamic entry");
			break;
		case PT_SCE_DYNLIBDATA:
			moduleInfo.m_pSceDynamicLib.m_pAddress = m_module->m_elfData.data() + pElfpHdr[index].p_offset;
			moduleInfo.m_pSceDynamicLib.m_size = pElfpHdr[index].p_filesz;
			break;
		default:
			PSET_LOG_INFO("unknown program header type");
		}
	}
}

void CPtElfProcessor::ParseDynamicSegment()
{
	SPtModuleInfo& moduleInfo = m_module->m_moduleInfo;
	uint8_t* baseAddress = (uint8_t*)moduleInfo.m_pSceDynamicLib.m_pAddress;
	for (uint32_t index = 0; index < moduleInfo.m_nDynamicEntryCount; index++)
	{
		Elf64_Dyn& elf64Dyn = ((Elf64_Dyn*)moduleInfo.m_pDynamicEntry)[index];
		switch (elf64Dyn.d_tag)
		{
		case DT_INIT:
			moduleInfo.m_nInitOffset = elf64Dyn.d_un.d_ptr;
			break;
		case DT_SCE_STRTAB:
			moduleInfo.m_sceStrTable.m_pAddress = baseAddress + elf64Dyn.d_un.d_ptr;
		case DT_SCE_STRSZ:
			moduleInfo.m_sceStrTable.m_size = elf64Dyn.d_un.d_val;
			break;
		case DT_SCE_RELA:
			moduleInfo.m_relocationTable.m_pAddress = baseAddress + elf64Dyn.d_un.d_ptr;;
			break;
		case DT_SCE_RELASZ:
			moduleInfo.m_relocationTable.m_size = elf64Dyn.d_un.d_val / sizeof(Elf64_Rela);
			break;
		case DT_SCE_JMPREL:
			moduleInfo.m_relocationPltTable.m_pAddress = baseAddress + elf64Dyn.d_un.d_ptr;;
			break;
		case DT_SCE_PLTRELSZ:
			moduleInfo.m_relocationPltTable.m_size = elf64Dyn.d_un.d_val / sizeof(Elf64_Rela);
			break;
		case DT_SCE_SYMTAB:
			moduleInfo.m_symbleTable.m_pAddress = baseAddress + elf64Dyn.d_un.d_ptr;
			break;
		case DT_SCE_SYMTABSZ:
			moduleInfo.m_symbleTable.m_size = elf64Dyn.d_un.d_val;
			break;
		}
	}

	uint8_t* sceStrTable = (uint8_t*)moduleInfo.m_sceStrTable.m_pAddress;
	for (uint32_t index = 0; index < moduleInfo.m_nDynamicEntryCount; index++)
	{
		Elf64_Dyn& elf64Dyn = ((Elf64_Dyn*)moduleInfo.m_pDynamicEntry)[index];
		switch (elf64Dyn.d_tag)
		{ 
		case DT_NEEDED:
		{
			char* fileName = (char*)&sceStrTable[elf64Dyn.d_un.d_ptr];
			m_module->m_neededFiles.push_back(fileName);

			PSET_LOG_INFO(std::string("DT_NEEDED:") + fileName);
			break;
		}

		case DT_SCE_MODULE_INFO:
		case DT_SCE_NEEDED_MODULE:
		{
			SSPtModuleValue moduleValue;
			moduleValue.value = elf64Dyn.d_un.d_val;
			m_module->m_id2ModuleNameMap.emplace(std::make_pair(moduleValue.id, (const char*)(&sceStrTable[moduleValue.name_offset])));
			break;
		}

		case DT_SCE_IMPORT_LIB:
		case DT_SCE_EXPORT_LIB:
		{
			SPtLibraryValue libValue;
			libValue.m_value = elf64Dyn.d_un.d_val;
			m_module->m_id2LibraryNameMap.emplace(std::make_pair(libValue.id, (const char*)(&sceStrTable[libValue.name_offset])));
			break;
		}

		case DT_SCE_ORIGINAL_FILENAME:
		{
			SPtLibraryValue libValue;
			libValue.m_value = elf64Dyn.d_un.d_val;
			PSET_LOG_INFO(std::string("DT_SCE_ORIGINAL_FILENAME:") + (char*)&sceStrTable[libValue.name_offset]);
			break;
		}
		}
	}
}

void CPtElfProcessor::MapImageToMemory()
{
	SMemoryChrunk& mappedMemory = m_module->m_moduleInfo.m_mappedMemory;
	mappedMemory.m_size = CalculateTotalLoadableSize();

	mappedMemory.m_pAddress = (void*)0x000000000;
	if (m_module->m_fileName == "eboot.bin")
	{
		mappedMemory.m_pAddress = (void*)0x800000000;
	}
	mappedMemory.m_pAddress = VirtualAlloc(mappedMemory.m_pAddress, mappedMemory.m_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	
	assert(mappedMemory.m_pAddress != nullptr);
	assert(reinterpret_cast<uint64_t>(mappedMemory.m_pAddress) % (16 * 1024) == 0);
	
	for (auto const& phdr : m_module->m_aSegmentHeaders)
	{
		switch (phdr.p_type)
		{
			case PT_LOAD:
			{
				if (phdr.p_flags & PF_X)
				{
					MapSegment(phdr);
				}
				else if (phdr.p_flags & PF_W)
				{
					MapSegment(phdr);
				}
				break;
			}
			case PT_SCE_RELRO:
			{
				MapSegment(phdr);
				break;
			}
		}
	}

	SPtModuleInfo& moduleInfo = m_module->m_moduleInfo;
	if (m_module->m_self64Ehdr.e_entry != 0)
	{
		moduleInfo.m_pEntryPoint = (uint8_t*)moduleInfo.m_mappedMemory.m_pAddress + m_module->m_self64Ehdr.e_entry;
	}
	else
	{
		moduleInfo.m_pEntryPoint = nullptr;
	}

	moduleInfo.m_pInitProc = moduleInfo.m_nInitOffset + (uint8_t*)moduleInfo.m_mappedMemory.m_pAddress;
	moduleInfo.m_pProcParamProc = moduleInfo.m_nProcParamOffset +(uint8_t*)moduleInfo.m_mappedMemory.m_pAddress;
}

void CPtElfProcessor::ParseAndExportNativeSymbol()
{
	SPtModuleInfo& moduleInfo = m_module->m_moduleInfo;
	RelocateNativeLocalSymbol((Elf64_Rela*)moduleInfo.m_relocationTable.m_pAddress, moduleInfo.m_relocationTable.m_size);
	RelocateNativeLocalSymbol((Elf64_Rela*)moduleInfo.m_relocationPltTable.m_pAddress, moduleInfo.m_relocationPltTable.m_size);

	uint8_t* pCodeAddress = (uint8_t*)moduleInfo.m_mappedMemory.m_pAddress;
	uint8_t* pStrTable = (uint8_t*)moduleInfo.m_sceStrTable.m_pAddress;
	Elf64_Sym* pSymTable = (Elf64_Sym*)moduleInfo.m_symbleTable.m_pAddress;

	uint32_t nSymbolNum = moduleInfo.m_symbleTable.m_size/ sizeof(Elf64_Sym);

	for (uint32_t i = 0; i < nSymbolNum; i++)
	{
		const Elf64_Sym& symbol = pSymTable[i];
		auto binding = ELF64_ST_BIND(symbol.st_info);
		char* symName = (char*)(&pStrTable[symbol.st_name]);

		if ((binding == STB_GLOBAL || binding == STB_WEAK) && symbol.st_shndx != SHN_UNDEF)
		{
			uint16_t moduleId = 0;
			uint16_t libId = 0;
			uint64_t nid = 0;
			CPtDynamicLinker::DecodeSymbol(symName, moduleId, libId, nid);
			std::string moduleName;
			std::string libName;

			auto iterModule = m_module->m_id2ModuleNameMap.find(moduleId);
			if (iterModule != m_module->m_id2ModuleNameMap.end())
			{
				moduleName = iterModule->second;
			}
			else
			{
				assert(false);
			}

			auto iterLib = m_module->m_id2LibraryNameMap.find(libId);
			if (iterLib != m_module->m_id2LibraryNameMap.end())
			{
				libName = iterLib->second;
			}
			else
			{
				assert(false);
			}
			GetPtDynamicLinker()->AddNativeModule(moduleName);
			GetPtDynamicLinker()->AddSymbol(moduleName, libName, nid, pCodeAddress + symbol.st_value, nullptr);
		}
	}
}

void CPtElfProcessor::AddModuleDependencies()
{
	for (auto& t : m_module->m_neededFiles)
	{
		GetElfModuleLoder()->AddModuleDependencies(t);
	}
}

static bool IsSegmentLoadable(Elf64_Phdr const& hdr)
{
	bool retVal = false;
	if (hdr.p_type == PT_SCE_RELRO || hdr.p_type == PT_LOAD)
	{
		retVal = true;
	}
	return retVal;
}

size_t CPtElfProcessor::CalculateTotalLoadableSize()
{
	size_t loadAddrBegin = 0;
	size_t loadAddrEnd = 0;

	for (auto& phdr : m_module->m_aSegmentHeaders)
	{
		if (IsSegmentLoadable(phdr))
		{
			if (phdr.p_vaddr < loadAddrBegin)
			{
				loadAddrBegin = phdr.p_vaddr;
			}

			size_t alignedAddr = AlignUp(phdr.p_vaddr + phdr.p_memsz, phdr.p_align);
			if (alignedAddr > loadAddrEnd)
			{
				loadAddrEnd = alignedAddr;
			}
		}
	}

	return (loadAddrEnd - loadAddrBegin);
}

void CPtElfProcessor::MapSegment(Elf64_Phdr const& hdr)
{
	SPtModuleInfo& moduleInfo = m_module->m_moduleInfo;
	uint8_t* pDstAddr = reinterpret_cast<uint8_t*>(AlignUp(size_t(moduleInfo.m_mappedMemory.m_pAddress) + hdr.p_vaddr, hdr.p_align));
	uint8_t* pSrcData = m_module->m_elfData.data() + hdr.p_offset;
	memcpy(pDstAddr, pSrcData, hdr.p_filesz);
}

void CPtElfProcessor::RelocateNativeLocalSymbol(Elf64_Rela* pReallocateTable, uint32_t relaCount)
{
	SPtModuleInfo& moduleInfo = m_module->m_moduleInfo;
	uint8_t* pCodeAddress = (uint8_t*)moduleInfo.m_mappedMemory.m_pAddress;
	Elf64_Sym* pSymTable = (Elf64_Sym*)moduleInfo.m_symbleTable.m_pAddress;

	for (uint32_t index = 0; index < relaCount; index++)
	{
		Elf64_Rela* pRela = &pReallocateTable[index];
		
		const uint64_t nSymIdx = ELF64_R_SYM(pRela->r_info);
		const uint64_t bindType = ELF64_R_TYPE(pRela->r_info);

		Elf64_Sym& symbol = pSymTable[nSymIdx];
		
		uint64_t symAddend = pRela->r_addend;
		uint64_t symValue = symbol.st_value;
		
		void* symAddress = nullptr;
		switch (bindType)
		{
		case R_X86_64_RELATIVE:
		{
			symAddress = pCodeAddress + symAddend;
			break;
		}
		case R_X86_64_JUMP_SLOT:
		case R_X86_64_GLOB_DAT:
		{
			symAddress = pCodeAddress + symValue;
			break;
		}
		case R_X86_64_64:
		{
			symAddress = pCodeAddress + symValue + symAddend;
			break;
		}
		}

		int nBinding = ELF64_ST_BIND(symbol.st_info);
		if (nBinding == STB_LOCAL)
		{
			*(uint64_t*)(pCodeAddress + pRela->r_offset) = reinterpret_cast<uint64_t>(symAddress);
		}
	}

}
