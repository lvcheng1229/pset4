#pragma once
#include <string>
#include <queue>
#include <set>

#include "PsetModule.h"
#include "PsetElf.h"
#include "PsetDynamicLinker.h"

class CPsetApplication;
class CPestModuleLoader
{
public:
	CPestModuleLoader(CPsetDynamicLinker* pDynamicLinker, CPsetApplication* pPsetApp);
	bool LoadModule(const std::string& modulePath, const std::string& moduleName);
	void LoadDependencies();
private:
	bool GetPathFromModuleName(const std::string& moduleName, std::string& outPath);
private:
	CElfProcessor m_psetElf;
	std::queue<std::string> m_penddingFileToLoad;

	std::set<std::string> m_loadedModuleNames;

	CPsetDynamicLinker* m_pDynamicLinker;
	CPsetApplication* m_psetApp;
};