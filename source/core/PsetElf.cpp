#include <iostream>

#include "system/elf.h"

#include "core/Log.h"
#include "PsetElf.h"

#include "core/PsetModule.h"
#include "core/PsetDynamicLinker.h"

void ValidateElf(elf64_hdr* elf64)
{
	Elf64_Half elfType = elf64->e_type;
	
	PSET_EXIT_AND_LOG_IF(strncmp((const char*)elf64->e_ident, ELFMAG, 4), "elf file magic number mismatch");
	PSET_EXIT_AND_LOG_IF(elfType != ET_SCE_EXEC && elfType != ET_SCE_REPLAY_EXEC && elfType != ET_SCE_RELEXEC && elfType != ET_SCE_DYNEXEC && elfType != ET_SCE_DYNAMIC, "unsupported type");
	PSET_EXIT_AND_LOG_IF(elf64->e_machine != EM_X86_64, "unsupported machine");
	PSET_EXIT_AND_LOG_IF(elf64->e_phnum == 0,"the number of entries in the program header table is zero");
}

//https://www.psdevwiki.com/ps4/SELF_File_Format#SELF_Decrypter_on_PS4
uint32_t FlagsId(uint64_t flag) 
{
	return (flag >> 20) & 0xFFF; 
}

CElfProcessor::~CElfProcessor()
{
	if (m_elf.m_pAddress)
	{
		free(m_elf.m_pAddress);
	}
}

void CElfProcessor::LoadFromFile(const std::string& elfPath, CPsetModule* psetModule, bool bEBootModule)
{
	m_moduleToLoad = psetModule;
	bEbootModule = bEBootModule;

	std::ifstream fin(elfPath, std::ifstream::in | std::ifstream::ate | std::ifstream::binary);
	PSET_EXIT_AND_LOG_IF(!fin.is_open(), "couldn't open app");

	size_t nFileSize = (size_t)fin.tellg();
	std::vector<uint8_t>fileRawData;
	fileRawData.resize(nFileSize);

	fin.seekg(0);
	fin.read((char*)fileRawData.data(), nFileSize);
	fin.close();

	SHeaderSelf* pHeaderSelf = (SHeaderSelf*)fileRawData.data();
	
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
			PSET_EXIT_AND_LOG_IF((pSegStructureSelf.m_flags == uint64_t(ESegFlags::SF_ENCR) || pSegStructureSelf.m_flags == uint64_t(ESegFlags::SF_DFLG)),"unsupport format");
		}

		elf64_hdr* elf64Header = (elf64_hdr*)(pSegStructureSelfs + numSegments);
		ValidateElf(elf64Header);

		m_moduleToLoad->m_elf64Ehdr = *elf64Header;

		elf64_phdr* elf64ProgramHeader = (elf64_phdr*)(elf64Header + 1);

		psetModule->m_aSegmentHeaders.resize(elf64Header->e_phnum);
		for (uint32_t index = 0; index < elf64Header->e_phnum; index++)
		{
			psetModule->m_aSegmentHeaders[index] = elf64ProgramHeader[index];
		}
		

		uint32_t segmentsStart = std::numeric_limits<uint32_t>::max();
		uint32_t segmentsEnd = 0;
		
		for (uint32_t index = 0; index < elf64Header->e_phnum; index++)
		{
			uint32_t segmentStart = elf64ProgramHeader[index].p_offset; // the offset start from the [elf] header instead of [self] header
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
		m_elf.m_pAddress = malloc(segmentsEnd);
		m_elf.m_size = segmentsEnd;
		memcpy(m_elf.m_pAddress, elf64Header, segmentsStart);

		for (uint32_t index = 0; index < numSegments; index++)
		{
			// we only take care of the data in the block segments
			if ((pSegStructureSelfs[index].m_flags & uint64_t(ESegFlags::SF_BFLG)) != 0)
			{
				uint32_t flagID = FlagsId(pSegStructureSelfs[index].m_flags);
				memcpy(
					(uint8_t*)m_elf.m_pAddress + elf64ProgramHeader[flagID].p_offset, 
					(uint8_t*)fileRawData.data() + pSegStructureSelfs[index].m_offsets,
					pSegStructureSelfs[index].m_encryptedCompressedSize);
			}
		}
		
		m_fileName = elfPath;

	}
	else
	{
		PSET_EXIT_AND_LOG("unsupported format");
	}
}


void CElfProcessor::MapImageIntoMemory()
{
	SMemoryChrunk& mappedMemory = m_moduleToLoad->m_moduleInfo.m_mappedMemory;
	mappedMemory.m_size =CalculateTotalLoadableSize();

	mappedMemory.m_pAddress = (void*)0x000000000;
	if (bEbootModule)
	{
		mappedMemory.m_pAddress = (void*)0x800000000;
		mappedMemory.m_pAddress = VirtualAlloc(mappedMemory.m_pAddress, mappedMemory.m_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}
	else
	{
		mappedMemory.m_pAddress = VirtualAlloc(mappedMemory.m_pAddress, mappedMemory.m_size,MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}

	
	assert(mappedMemory.m_pAddress != nullptr);
	for (auto const& phdr : m_moduleToLoad->m_aSegmentHeaders)
	{
		bool result = true;
		switch (phdr.p_type)
		{
			case PT_LOAD:
			{
				if (phdr.p_flags & PF_X)
				{
					result = MapSegment(phdr);
				}
				else if (phdr.p_flags & PF_W)
				{
					result = MapSegment(phdr);
				}
				break;
			}
			case PT_SCE_RELRO:
			{
				result = MapSegment(phdr);
				break;
			}
			case PT_DYNAMIC:
			{
				//TODO
				break;
			}
			case PT_SCE_PROCPARAM:
			case PT_TLS:
			{
				//TODO:
				break;
			}
			case PT_GNU_EH_FRAME:
			{
				//TODO:
				break;
			}
		}
	}

	SModuleInfo& moduleInfo = m_moduleToLoad->m_moduleInfo;
	if (m_moduleToLoad->m_elf64Ehdr.e_entry != 0)
	{
		moduleInfo.pEntryPoint = (uint8_t*)moduleInfo.m_mappedMemory.m_pAddress + m_moduleToLoad->m_elf64Ehdr.e_entry;
	}
	else
	{
		moduleInfo.pEntryPoint = nullptr;
	}
	
}

void CElfProcessor::PrepareProgramHeader()
{
	SModuleInfo& moduleInfo = m_moduleToLoad->m_moduleInfo;

	elf64_hdr* pElfHdr = (elf64_hdr*)(m_elf.m_pAddress);
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
			m_pProcParam = (void*)pElfpHdr[index].p_paddr;
			break;
		case PT_SCE_MODULEPARAM:
			m_pModuleParam = (void*)pElfpHdr[index].p_paddr;
			break;
		case PT_INTERP:
			m_pInterProgram = ((uint8_t*)m_elf.m_pAddress) + pElfpHdr[index].p_offset;
			break;
		case PT_DYNAMIC:
			m_pDynamicEntry = (Elf64_Dyn*)((uint8_t*)m_elf.m_pAddress + pElfpHdr[index].p_offset);
			m_nDynamicEntryCount = pElfpHdr[index].p_filesz / sizeof(Elf64_Dyn);
			PSET_EXIT_AND_LOG_IF(m_nDynamicEntryCount == 0, "empty dynamic entry");
			break;
		case PT_SCE_DYNLIBDATA:
			moduleInfo.m_pSceDynamicLib.m_pAddress = (uint8_t*)m_elf.m_pAddress + pElfpHdr[index].p_offset;
			moduleInfo.m_pSceDynamicLib.m_size = pElfpHdr[index].p_filesz;
			break;
		case PT_TLS:
			m_tlsBlock.m_tmpStatrt = pElfpHdr[index].p_paddr;
			m_tlsBlock.m_tmpSize = pElfpHdr[index].p_filesz;
			m_tlsBlock.m_fullSize = pElfpHdr[index].p_memsz;
			m_tlsBlock.m_align = pElfpHdr[index].p_align;
			m_tlsBlock.m_offset = -m_tlsBlock.m_fullSize;
			m_tlsBlock.m_index = uint64_t(this);
			break;
		case PT_GNU_EH_FRAME:
			m_ehFrameHeader.m_address = pElfpHdr[index].p_vaddr;
			m_ehFrameHeader.m_size = pElfpHdr[index].p_filesz;
			break;
		default:
			PSET_LOG_INFO("unknown program header type");
		}
	}
}

//COMMONT:PAGE204
void CElfProcessor::PrepreDynamicSegments()
{
	for (uint32_t index = 0; index < m_nDynamicEntryCount; index++)
	{
		PrepareTables(m_pDynamicEntry[index]);
	}

	for (uint32_t index = 0; index < m_nDynamicEntryCount; index++)
	{
		ParseSingleDynamicEntry(m_pDynamicEntry[index]);
	}
}

std::vector<std::string>& CElfProcessor::GetNeededFiles()
{
	return m_aNeededFiles;
}

//TODO: Rename TO PrePareDynamic Segments SUB 0
void CElfProcessor::PrepareTables(Elf64_Dyn& elf64Dyn)
{
	SModuleInfo& moduleInfo = m_moduleToLoad->m_moduleInfo;
	uint8_t* baseAddress = (uint8_t*)moduleInfo.m_pSceDynamicLib.m_pAddress;
	switch (elf64Dyn.d_tag)
	{
	case DT_NULL:
	case DT_NEEDED:
	case DT_DEBUG:
	case DT_TEXTREL:
	case DT_INIT_ARRAY:
	case DT_FINI_ARRAY:
	case DT_INIT_ARRAYSZ:
	case DT_FINI_ARRAYSZ:
	case DT_FLAGS:
	case DT_PREINIT_ARRAY:
	case DT_PREINIT_ARRAYSZ:
	case DT_SCE_FINGERPRINT:
	case DT_SCE_ORIGINAL_FILENAME:
	case DT_SCE_MODULE_INFO:
	case DT_SCE_NEEDED_MODULE:
	case DT_SCE_MODULE_ATTR:
	case DT_SCE_EXPORT_LIB:
	case DT_SCE_IMPORT_LIB:
	case DT_SCE_EXPORT_LIB_ATTR:
	case DT_SCE_IMPORT_LIB_ATTR:
	case DT_SCE_RELAENT:
	case DT_SCE_SYMENT:
	case DT_SCE_HASH:
	case DT_SCE_HASHSZ:
	case DT_SONAME:
		break;

	case DT_INIT:
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
	case DT_SCE_PLTREL:
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

//TODO: Rename TO PrePareDynamic Segments SUB 1
void CElfProcessor::ParseSingleDynamicEntry(Elf64_Dyn& elf64Dyn)
{
	SModuleInfo& moduleInfo = m_moduleToLoad->m_moduleInfo;
	uint8_t* sceStrTable = (uint8_t*)moduleInfo.m_sceStrTable.m_pAddress;

	switch (elf64Dyn.d_tag)
	{
		case DT_NEEDED:
		{
			//COMMONT:PAGE205
			char* fileName = (char*)&sceStrTable[elf64Dyn.d_un.d_ptr];
			m_aNeededFiles.push_back(fileName);

			PSET_LOG_INFO(std::string("DT_NEEDED:") + fileName);
			break;
		}

		case DT_SCE_MODULE_INFO:
		case DT_SCE_NEEDED_MODULE:
		{
			SModuleValue moduleValue;
			SPsetModule module;
			moduleValue.value = elf64Dyn.d_un.d_val;
			module.m_moduleName = reinterpret_cast<char*>(&sceStrTable[moduleValue.name_offset]);
			m_moduleToLoad->m_id2ModuleNameMap.emplace(std::make_pair(moduleValue.m_id, module));

			std::cout << module.m_moduleName << std::endl;
			break;
		}

		//TODO
		case DT_SCE_IMPORT_LIB:
		case DT_SCE_EXPORT_LIB:
		{
			SLibraryValue libValue;
			SPsetLibrary lib;
			libValue.m_value = elf64Dyn.d_un.d_val;
			lib.m_libraryName = (const char*)(&sceStrTable[libValue.m_name_offset]);
			m_moduleToLoad->m_id2LibraryNameMap.emplace(std::make_pair(libValue.m_id, lib));
			break;
		}

		case DT_SCE_ORIGINAL_FILENAME:
		{
			SLibraryValue libValue;
			libValue.m_value = elf64Dyn.d_un.d_val;
			PSET_LOG_INFO(std::string("DT_SCE_ORIGINAL_FILENAME:") + (char*)&sceStrTable[libValue.m_name_offset]);
			break;
		}
	}
}

void CElfProcessor::MapCodeInit()
{

}

bool CElfProcessor::MapSegment(Elf64_Phdr const& hdr)
{
	SModuleInfo& moduleInfo = m_moduleToLoad->m_moduleInfo;
	uint8_t* pDstAddr = reinterpret_cast<uint8_t*>(AlignUp(size_t(moduleInfo.m_mappedMemory.m_pAddress) + hdr.p_vaddr, hdr.p_align));
	uint8_t* pSrcData = (uint8_t*)m_elf.m_pAddress + hdr.p_offset;
	memcpy(pDstAddr, pSrcData, hdr.p_filesz);
	return true;
}



size_t CElfProcessor::CalculateTotalLoadableSize()
{
	size_t loadAddrBegin = 0;
	size_t loadAddrEnd = 0;

	for (auto& phdr : m_moduleToLoad->m_aSegmentHeaders)
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

bool CElfProcessor::IsSegmentLoadable(Elf64_Phdr const& hdr)
{
	bool retVal;

	if (hdr.p_type == PT_SCE_RELRO)
	{
		retVal = true;
	}
	else if (hdr.p_type == PT_LOAD)
	{
		retVal = true;
	}
	else
	{
		retVal = false;
	}

	return retVal;
}
