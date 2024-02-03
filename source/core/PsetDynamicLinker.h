#pragma once
#include <map>
#include <string>

#include "PsetModule.h"
class CPsetDynamicLinker
{
public:

	void AddNeededModule(std::string const& moduleName, CPsetModule&& module);;

	void AddOverrideModule();

	bool IsOverrideModule(std::string const& moduleName);
	bool RelocateModules(CPsetModule& module);
private:
	bool RelocateRelative(CPsetModule& module);
	bool RelocatePlatformRelative(CPsetModule& module);
private:
	std::map<std::string, size_t>m_mapModuleNameToIndex;
	std::vector<CPsetModule> m_aGloabalModules;
};

extern CPsetDynamicLinker* GetDynamicLinker();
