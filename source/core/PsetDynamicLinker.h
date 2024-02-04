#pragma once
#include <map>
#include <string>
#include <unordered_map>

#include "PsetModule.h"
#include "overridelib/PsetLibraryCommon.h"


class CPsetDynamicLinker
{
public:
	void AddNeededModule(std::string const& moduleName, CPsetModule&& module);;

	void InitializeOverrideModule(const char* moduleName, const SPSET_EXPORT_LIB* pLibraries);
	void AddOverrideModule(const std::string& moduleName);
	void AddOverrideSymbol(const std::string& moduleName, const std::string& libraryName, const uint64_t nid, const void* funcAddress);

	bool IsOverrideModule(std::string const& moduleName);
	bool RelocateModules(CPsetModule& module);

	const void* GetOverrideSymbolAddress(const std::string& moduleName, const std::string& libraryName, const uint64_t nid);

	void GetLibAndModuleName(CPsetModule& module, uint16_t libId, uint16_t moduleId, std::string& outlibName, std::string& outModulebName);
private:
	bool RelocateRelative(CPsetModule& module);
	bool RelocatePlatformRelative(CPsetModule& module);
private:
	std::map<std::string, size_t>m_mapModuleNameToIndex;
	std::vector<CPsetModule> m_aGloabalModules;


	using SNid2FunctionMap = std::unordered_map<uint64_t, const void*>;
	using SLib2FuncTableMap = std::unordered_map < std::string, SNid2FunctionMap>;
	using SStr2ModuleMap = std::unordered_map < std::string, SLib2FuncTableMap>;

	SNid2FunctionMap m_nid2FunctionMap;
	SLib2FuncTableMap m_lib2FunctionTableMap;
	SStr2ModuleMap m_str2ModuleMap;

	std::vector<std::string> m_overrideModules;
};

extern CPsetDynamicLinker* GetDynamicLinker();
