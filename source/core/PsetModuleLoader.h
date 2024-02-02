#pragma once
#include <string>
#include <queue>

#include "PsetModule.h"
#include "PsetElf.h"
class CPestModuleLoader
{
public:
	CPestModuleLoader();
	bool LoadModule(std::string const& fileName, CPsetModule*& module);
	void LoadDependencies();
private:
	CPsetElf m_psetElf;
	std::queue<std::string> m_penddingFileToLoad;
};