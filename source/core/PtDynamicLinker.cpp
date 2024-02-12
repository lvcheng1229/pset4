#include "PtDynamicLinker.h"
#include "PtInvalidFuncLogger.h"

static CPtDynamicLinker* g_pDynamicLinker;
CPtDynamicLinker* GetPtDynamicLinker()
{
	if (g_pDynamicLinker == nullptr)
	{
		g_pDynamicLinker = new CPtDynamicLinker();
	}
	return g_pDynamicLinker;
}

void CPtDynamicLinker::InitializeOverrideModule(const char* moduleName, const SPSET_EXPORT_LIB* pLibraries)
{
	const std::string moduleNameStr(moduleName);

	auto moduleIter = m_str2ModuleMap.find(moduleNameStr);
	if (moduleIter == m_str2ModuleMap.end())
	{
		m_str2ModuleMap.emplace(std::make_pair(moduleNameStr, SLib2FuncTableMap()));
		AddOverrideModule(moduleNameStr);
	}

	uint32_t libIndex = 0;
	while (pLibraries[libIndex].m_libName != nullptr && pLibraries[libIndex].m_pFunctionEntries != nullptr)
	{
		const std::string& libName = pLibraries[libIndex].m_libName;
		const SPSET_LIB_EXPORT_SYSMBOL* pFunctionEntries = pLibraries[libIndex].m_pFunctionEntries;
		uint32_t funcIndex = 0;
		while (pFunctionEntries[funcIndex].m_nid != 0 && pFunctionEntries[funcIndex].m_pFunction != nullptr)
		{
			const uint64_t nid = pFunctionEntries[funcIndex].m_nid;

			AddSymbol(moduleNameStr, libName, nid, pFunctionEntries[funcIndex].m_pFunction, pFunctionEntries[funcIndex].m_funcName);
			funcIndex++;
		}
		libIndex++;
	}
}

bool CPtDynamicLinker::IsNativeModule(const std::string& filename)
{
	for (auto& t : m_overrideModules)
	{
		if ((t + ".prx") == filename)
		{
			return false;
		}
	}
	return true;
}

void CPtDynamicLinker::RelocateNativeModules(std::vector<CPtNativeModule>& nativeModules,uint32_t moduleNum)
{
	for (uint32_t index = 0; index < moduleNum; index++)
	{
		SPtModuleInfo& moduleInfo = nativeModules[index].GetModuleInfo();
		RelocateRelativeSymbols(nativeModules[index], (Elf64_Rela*)moduleInfo.m_relocationTable.m_pAddress, moduleInfo.m_relocationTable.m_size);
		RelocateRelativeSymbols(nativeModules[index], (Elf64_Rela*)moduleInfo.m_relocationPltTable.m_pAddress, moduleInfo.m_relocationPltTable.m_size);
	}
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
	assert(encodedValue.length() <= nMaxEncodedLength);

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

void CPtDynamicLinker::AddNativeModule(const std::string& moduleName)
{
	const std::string moduleNameStr(moduleName);

	auto moduleIter = m_str2ModuleMap.find(moduleNameStr);
	if (moduleIter == m_str2ModuleMap.end())
	{
		m_str2ModuleMap.emplace(std::make_pair(moduleNameStr, SLib2FuncTableMap()));
	}
}

static bool IsEncodedSymbol(std::string const& symbolName) 
{
	bool retVal = false;
	if (symbolName.length() != 15)
	{
		retVal = false;
	}
	else if (symbolName[11] != '#' || symbolName[13] != '#')
	{
		retVal = false;
	}
	else
	{
		retVal = true;
	}

	return retVal;
}

void CPtDynamicLinker::DecodeSymbol(const std::string& encodedName, uint16_t& moduleId, uint16_t& libId, uint64_t& nid)
{
	moduleId = 0;
	libId = 0;
	nid = 0;

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
	encodedSubNames.push_back(inputEncodedName);

	DecodeValue64(encodedSubNames[0], nid);
	DecodeValue16(encodedSubNames[1], libId);
	DecodeValue16(encodedSubNames[2], moduleId);
}




void CPtDynamicLinker::RelocateRelativeSymbols(CPtNativeModule& nativeModule, Elf64_Rela* pReallocateTable, uint32_t relaCount)
{
	SPtModuleInfo& moduleInfo = nativeModule.GetModuleInfo();
	uint8_t* pCodeAddress = (uint8_t*)moduleInfo.m_mappedMemory.m_pAddress;
	uint8_t* pStrTable = (uint8_t*)moduleInfo.m_sceStrTable.m_pAddress;
	Elf64_Sym* pSymTable = (Elf64_Sym*)moduleInfo.m_symbleTable.m_pAddress;

	for (uint32_t index = 0; index < relaCount; index++)
	{
		Elf64_Rela* pRela = &pReallocateTable[index];
		const uint64_t nSymIdx = ELF64_R_SYM(pRela->r_info);

		Elf64_Sym& symbol = pSymTable[nSymIdx];
		auto nBinding = ELF64_ST_BIND(symbol.st_info);
		const char* pEncodedName = (const char*)&pStrTable[symbol.st_name];

		if (nBinding == STB_GLOBAL || nBinding == STB_WEAK)
		{
			if (!IsEncodedSymbol(pEncodedName))
			{
				continue;
			}
			uint64_t decodedNid = 0;
			uint16_t decodedModuleId = 0;
			uint16_t decodedLibId = 0;
			DecodeSymbol(pEncodedName, decodedModuleId, decodedLibId, decodedNid);

			std::string libName;
			std::string moduleName;
			nativeModule.GetLibAndModuleName(decodedLibId, decodedModuleId, libName, moduleName);
			
			bool bSymValid = IsSymbolValid(moduleName, libName, decodedNid);

			std::string reDirlibName = libName;
			std::string reDirmoduleName = moduleName;

			if (!bSymValid)
			{
				reDirlibName = "libc";
				reDirmoduleName = "libc";
			}

			const void* symAddress = GetSymbolAddress(reDirmoduleName, reDirlibName, decodedNid);
			if (symAddress)
			{
				*(uint64_t*)(pCodeAddress + pRela->r_offset) = reinterpret_cast<uint64_t>(symAddress);
				bSymValid = true;
			}
			else
			{
				printf("NID:");
				printf("%X", uint32_t((decodedNid >> 32) & 0xffffffff));
				printf("%X", uint32_t(decodedNid & 0xffffffff));
				PSET_LOG_ERROR("Invalid symbol Module name:" + moduleName + " LibName:" + libName);

				bSymValid = false;
			}

			if (!bSymValid)
			{
				*(uint64_t*)(pCodeAddress + pRela->r_offset) = reinterpret_cast<uint64_t>(GenStubFunc(std::format("{:X}", decodedNid)));
			}
		}
	}
}

const void* CPtDynamicLinker::GetSymbolAddress(const std::string& moduleName, const std::string& libraryName, const uint64_t nid)
{
	auto lib2FunctionTableMap = m_str2ModuleMap.find(moduleName);
	if (lib2FunctionTableMap == m_str2ModuleMap.end())
	{
		return nullptr;
	}

	auto nid2FunctionMap = lib2FunctionTableMap->second.find(libraryName);
	if (nid2FunctionMap == lib2FunctionTableMap->second.end())
	{
		return nullptr;
	}
	auto funcIter = nid2FunctionMap->second.find(nid);
	if (funcIter == nid2FunctionMap->second.end())
	{
		return nullptr;
	}

	return funcIter->second.m_pFunc;
}

bool CPtDynamicLinker::IsSymbolValid(const std::string& moduleName, const std::string& libraryName, const uint64_t nid)
{
	auto lib2FunctionTableMap = m_str2ModuleMap.find(moduleName);
	if (lib2FunctionTableMap == m_str2ModuleMap.end())
	{
		return false;
	}

	auto nid2FunctionMap = lib2FunctionTableMap->second.find(libraryName);
	if (nid2FunctionMap == lib2FunctionTableMap->second.end())
	{
		return false;
	}
	auto funcIter = nid2FunctionMap->second.find(nid);
	if (funcIter == nid2FunctionMap->second.end())
	{
		return false;
	}
	return funcIter->second.m_pFunc != nullptr;
}

void CPtDynamicLinker::AddOverrideModule(const std::string& moduleName)
{
	m_overrideModules.push_back(moduleName);
}

void CPtDynamicLinker::AddSymbol(const std::string& moduleName, const std::string& libraryName, const uint64_t nid, const void* funcAddress, const char* funcName)
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
		nid2FunctionMap.emplace(std::make_pair(nid, SFunctionInfo{ funcAddress,funcName }));
	}
}
