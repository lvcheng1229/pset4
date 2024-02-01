#include "PsetModuleLoader.h"

CPestModuleLoader::CPestModuleLoader()
{
}

bool CPestModuleLoader::LoadModule(std::string const& fileName, CPsetModule*& pesetModule)
{
	m_psetElf.LoadFromFile(fileName, pesetModule);
	m_psetElf.MapImageIntoMemory();
	

	return false;
}
