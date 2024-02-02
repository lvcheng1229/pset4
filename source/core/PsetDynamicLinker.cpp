#include "PsetDynamicLinker.h"

void CPsetDynamicLinker::AddModule(std::string const& moduleName, CPsetModule&& module)
{
	size_t index = m_aGloabalModules.size();
	m_mapModuleNameToIndex.emplace(std::make_pair(moduleName, index));
	m_aGloabalModules.emplace_back(std::move(module));
}
