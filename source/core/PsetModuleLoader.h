#pragma once
#include <string>

#include "PsetModule.h"
#include "PsetElf.h"
class CPestModuleLoader
{
public:
	CPestModuleLoader();
	bool LoadModule(std::string const& fileName, CPsetModule*& module);
private:
	CPsetElf m_psetElf;
};