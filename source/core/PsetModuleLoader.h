#pragma once
#include <string>
#include <queue>

#include "PsetModule.h"
#include "PsetElf.h"
#include "PsetDynamicLinker.h"

class CPsetApplication;
class CPestModuleLoader
{
public:
	CPestModuleLoader(CPsetDynamicLinker* pDynamicLinker, CPsetApplication* pPsetApp);
	bool LoadModule(std::string const& fileName, CPsetModule*& module);
	void LoadDependencies();
private:
	bool GetPathFromModuleName(const std::string& moduleName, std::string& outPath);
private:
	CPsetElf m_psetElf;
	std::queue<std::string> m_penddingFileToLoad;

	CPsetDynamicLinker* m_pDynamicLinker;
	CPsetApplication* m_psetApp;
};