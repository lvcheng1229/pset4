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

void CPsetDynamicLinker::InitializeOverrideModule(const char* moduleName, const SPSET_EXPORT_LIB* pLibraries)
{
	const std::string moduleNameStr(moduleName);

	auto moduleIter = m_str2ModuleMap.find(moduleNameStr);
	if (moduleIter == m_str2ModuleMap.end())
	{
		m_str2ModuleMap.emplace(std::make_pair(moduleNameStr, SLib2FuncTableMap()));
		AddOverrideModule(moduleNameStr);
	}

	uint32_t moduleIndex = 0;
	while (pLibraries[moduleIndex].m_libName != nullptr && pLibraries[moduleIndex].m_pFunctionEntries != nullptr)
	{
		const std::string& libName = pLibraries[moduleIndex].m_libName;
		const SPSET_LIB_EXPORT_SYSMBOL* pFunctionEntries = pLibraries[moduleIndex].m_pFunctionEntries;
		uint32_t funcIndex = 0;
		while (pFunctionEntries[funcIndex].m_nid != 0 && pFunctionEntries[funcIndex].m_pFunction != nullptr)
		{
			const uint64_t nid = pFunctionEntries[funcIndex].m_nid;
			AddOverrideSymbol(moduleNameStr, libName, nid, pFunctionEntries[funcIndex].m_pFunction);
			funcIndex++;
		}
		moduleIndex++;
	}
}

void CPsetDynamicLinker::AddOverrideModule(const std::string& moduleName)
{
	m_overrideModules.push_back(moduleName);
}

void CPsetDynamicLinker::AddOverrideSymbol(const std::string& moduleName, const std::string& libraryName, const uint64_t nid, const void* funcAddress)
{
	auto moduleIter = m_str2ModuleMap.find(moduleName);
	PSET_EXIT_AND_LOG_IF(moduleIter == m_str2ModuleMap.end(), "invalid module");

	SLib2FuncTableMap& lib2FuncTableMap = m_str2ModuleMap[moduleName];
	auto libIter = lib2FuncTableMap.find(libraryName);
	if (libIter == lib2FuncTableMap.end())
	{
		lib2FuncTableMap.emplace(std::make_pair(libraryName, SNid2FunctionMap()));
	}

	SNid2FunctionMap& nid2FunctionMap = lib2FuncTableMap[libraryName];
	auto funcIter = nid2FunctionMap.find(nid);
	if (funcIter == nid2FunctionMap.end())
	{
		nid2FunctionMap.emplace(std::make_pair(nid, funcAddress));
	}
}


bool CPsetDynamicLinker::IsOverrideModule(std::string const& moduleName)
{
	return false;
}

bool CPsetDynamicLinker::RelocateModules(CPsetModule& module)
{
	SModuleInfo& moduleInfo = module.GetModuleInfo();
	bool resultReloRelative = RelocateRelative(module, (Elf64_Rela*)moduleInfo.m_relocationTable.m_pAddress, moduleInfo.m_relocationTable.m_size);
	bool resultReloPltRelative = RelocateRelative(module, (Elf64_Rela*)moduleInfo.m_relocationPltTable.m_pAddress, moduleInfo.m_relocationPltTable.m_size);
	return resultReloRelative && resultReloPltRelative;
}

const void* CPsetDynamicLinker::GetOverrideSymbolAddress(const std::string& moduleName, const std::string& libraryName, const uint64_t nid)
{
	auto lib2FunctionTableMap = m_str2ModuleMap.find(moduleName);
	PSET_EXIT_AND_LOG_IF(lib2FunctionTableMap == m_str2ModuleMap.end(), "invalid override symbol address");

	auto nid2FunctionMap = lib2FunctionTableMap->second.find(libraryName);
	PSET_EXIT_AND_LOG_IF(nid2FunctionMap == lib2FunctionTableMap->second.end(), "invalid override symbol address");

	auto funcIter = nid2FunctionMap->second.find(nid);
	PSET_EXIT_AND_LOG_IF(funcIter == nid2FunctionMap->second.end(), "invalid override symbol address");

	return funcIter->second;
}

void CPsetDynamicLinker::GetLibAndModuleName(CPsetModule& module,uint16_t libId, uint16_t moduleId, std::string& outlibName, std::string& outModulebName)
{
	outlibName = module.GetId2LibMap()[libId].m_libraryName;
	outModulebName = module.GetId2ModuleMap()[moduleId].m_moduleName;
}

static bool DecodeValue16(const std::string& encodedValue, uint16_t& value)
{
	const uint32_t nMaxEncodedLength = 3;
	assert(encodedValue.length() < nMaxEncodedLength);

	const char pCodes[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
	bool bRetResult = true;
	for (int i = 0; i < encodedValue.size(); ++i)
	{
		auto pChPos = strchr(pCodes, encodedValue[i]);
		uint32_t nIndex = 0;

		if (pChPos != nullptr)
		{
			nIndex = static_cast<uint32_t>(pChPos - pCodes);
		}
		else
		{
			bRetResult = false;
			break;
		}

		if (i < nMaxEncodedLength - 1) // i = 0 - 2 , bit 0 - 11
		{
			value <<= 6;
			value |= nIndex;
		}
		else // i = 3 , bit 12 - 16
		{
			value <<= 4;
			value |= (nIndex >> 2);
		}
	}
	return bRetResult;
}

static bool DecodeValue64(const std::string& encodedValue, uint64_t& value)
{
	const uint32_t nMaxEncodedLength = 11;
	assert(encodedValue.length() < nMaxEncodedLength);

	const char pCodes[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
	bool bRetResult = true;
	for (int i = 0; i < encodedValue.size(); ++i)
	{
		auto pChPos = strchr(pCodes, encodedValue[i]);
		uint32_t nIndex = 0;

		if (pChPos != nullptr)
		{
			nIndex = static_cast<uint32_t>(pChPos - pCodes);
		}
		else
		{
			bRetResult = false;
			break;
		}
		
		if (i < nMaxEncodedLength - 1) // i = 0 - 9 , bit 0 - 59
		{
			value <<= 6;
			value |= nIndex;
		}
		else // i = 10 , bit 60 - 63
		{
			value <<= 4;
			value |= (nIndex >> 2);
		}
	}
	return bRetResult;
}

static void DecodeSymbol(const std::string& encodedName, uint16_t& moduleId, uint16_t& libId, uint64_t& nid)
{
	std::string inputEncodedName = encodedName;
	std::vector<std::string>encodedSubNames;
	const std::string delimiter = "#";

	size_t pos = 0;
	std::string token;
	while ((pos = inputEncodedName.find(delimiter)) != std::string::npos) 
	{
		encodedSubNames.push_back(inputEncodedName.substr(0, pos));
		inputEncodedName.erase(0, pos + delimiter.length());
	}

	DecodeValue64(encodedSubNames[0], nid);
	DecodeValue16(encodedSubNames[1], libId);
	DecodeValue16(encodedSubNames[2], moduleId);
}

bool CPsetDynamicLinker::RelocateRelative(CPsetModule& module, Elf64_Rela* pReallocateTable, uint32_t relaCount)
{
	SModuleInfo& moduleInfo = module.GetModuleInfo();
	uint8_t* pCodeAddress = (uint8_t*)moduleInfo.m_mappedCodeMemory.m_pAddress;
	uint8_t* pStrTable = (uint8_t*)moduleInfo.m_sceStrTable.m_pAddress;
	Elf64_Sym* pSymTable = (Elf64_Sym*)moduleInfo.m_symbleTable.m_pAddress;

	for (uint32_t index = 0; index < relaCount; index++)
	{
		//COMMONT:SYSVABI:PAGE69

		Elf64_Rela* pRela = &pReallocateTable[index];
		const uint64_t nSymIdx = ELF64_R_SYM(pRela->r_info);

		Elf64_Sym& symbol = pSymTable[nSymIdx];
		auto nBinding = ELF64_ST_BIND(symbol.st_info);
		const char* pEncodedName = (const char*)&pStrTable[symbol.st_name];
		
		if (nBinding == STB_GLOBAL || nBinding == STB_WEAK)
		{
			uint64_t decodedNid = 0;
			uint16_t decodedModuleId = 0;
			uint16_t decodedLibId = 0;
			DecodeSymbol(pEncodedName, decodedModuleId, decodedLibId, decodedNid);

			std::string libName;
			std::string moduleName;
			GetLibAndModuleName(module, decodedLibId, decodedModuleId, libName, moduleName);
			const void* symAddress = GetOverrideSymbolAddress(moduleName, libName, decodedLibId);
			uint64_t pCodeOffset = pRela->r_offset;

			*(uint64_t*)(pCodeAddress + pCodeOffset) = reinterpret_cast<uint64_t>(symAddress);
		}
	}

	return false;
}
