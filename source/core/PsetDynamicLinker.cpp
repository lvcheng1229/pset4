#include "PsetDynamicLinker.h"

static CPsetDynamicLinker* pDynamicLinker;
CPsetDynamicLinker* GetDynamicLinker()
{
	if (pDynamicLinker == nullptr)
	{
		pDynamicLinker = new CPsetDynamicLinker();
	}
	return pDynamicLinker;
}




void CPsetDynamicLinker::AddNeededModule(std::string const& moduleName, CPsetModule&& module)
{
	size_t index = m_aGloabalModules.size();
	m_mapModuleNameToIndex.emplace(std::make_pair(moduleName, index));
	m_aGloabalModules.emplace_back(std::move(module));
}


bool CPsetDynamicLinker::IsOverrideModule(std::string const& moduleName)
{
	return false;
}

bool CPsetDynamicLinker::RelocateModules(CPsetModule& module)
{
	return RelocateRelative(module);
}

bool CPsetDynamicLinker::RelocateRelative(CPsetModule& module)
{
	SModuleInfo& moduleInfo = module.GetModuleInfo();
	uint8_t* pCodeAddress = (uint8_t*)moduleInfo.m_mappedCodeMemory.m_pAddress;
	uint8_t* pStrTable = (uint8_t*)moduleInfo.m_sceStrTable.m_pAddress;
	Elf64_Rela* pReallocateTable = (Elf64_Rela*)moduleInfo.m_relocationTable.m_pAddress;
	Elf64_Sym* pSymTable = (Elf64_Sym*)moduleInfo.m_symbleTable.m_pAddress;

	for (uint32_t index = 0; index < moduleInfo.m_relocationTable.m_size; index++)
	{
		//COMMONT:SYSVABI:PAGE69

		Elf64_Rela* pRela = &pReallocateTable[index];
		const uint64_t nType = ELF64_R_TYPE(pRela->r_info);
		const uint64_t nSymIdx = ELF64_R_SYM(pRela->r_info);

		struct SSymbolInfo
		{
			uint64_t m_type; //relocation type ,example: R_X86_64_64
		};

		
		switch (nType)
		{

		}
	}

	return false;
}
