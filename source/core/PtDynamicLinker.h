#pragma once
#include "overridemodule/PsetLibraryCommon.h"
#include "PtNativeModule.h"

struct SFunctionInfo
{
	const void* m_pFunc;
	const char* m_funcName;
};

class CPtDynamicLinker
{
public:
	static void DecodeSymbol(const std::string& encodedName, uint16_t& moduleId, uint16_t& libId, uint64_t& nid);

	void AddNativeModule(const std::string& moduleName);
	
	void InitializeOverrideModule(const char* moduleName, const SPSET_EXPORT_LIB* pLibraries);
	void AddSymbol(const std::string& moduleName, const std::string& libraryName, const uint64_t nid, const void* funcAddress, const char* funcName);
	bool IsNativeModule(const std::string& filename);
	void RelocateNativeModules(std::vector<CPtNativeModule>& nativeModules,uint32_t moduleNum);
private:
	void AddOverrideModule(const std::string& moduleName);
	void RelocateRelativeSymbols(CPtNativeModule& nativeModule, Elf64_Rela* pReallocateTable, uint32_t relaCount);
	const void* GetSymbolAddress(const std::string& moduleName, const std::string& libraryName, const uint64_t nid);
	bool IsSymbolValid(const std::string& moduleName, const std::string& libraryName, const uint64_t nid);
private:
	using SNid2FunctionMap = std::unordered_map<uint64_t, SFunctionInfo>;
	using SLib2FuncTableMap = std::unordered_map < std::string, SNid2FunctionMap>;
	using SStr2ModuleMap = std::unordered_map < std::string, SLib2FuncTableMap>;

	SNid2FunctionMap m_nid2FunctionMap;
	SLib2FuncTableMap m_lib2FunctionTableMap;
	SStr2ModuleMap m_str2ModuleMap;

	std::vector<std::string> m_overrideModules;
};

CPtDynamicLinker* GetPtDynamicLinker();