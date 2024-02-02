#pragma once
#include <map>
#include <string>

#include "PsetModule.h"
class CPsetDynamicLinker
{
public:

	void AddModule(std::string const& moduleName, CPsetModule&& module);;

private:
	std::map<std::string, size_t>m_mapModuleNameToIndex;
	std::vector<CPsetModule> m_aGloabalModules;
};