#include "PsetModuleLoader.h"

CPestModuleLoader::CPestModuleLoader()
{
}

bool CPestModuleLoader::LoadModule(std::string const& fileName, CPsetModule*& pesetModule)
{
	CPsetModule outModule;
	m_psetElf.LoadFromFile(fileName, &outModule);
	m_psetElf.PrepareProgramHeader();
	m_psetElf.PrepreDynamicTables();
	m_psetElf.MapImageIntoMemory();

	for (auto& file : m_psetElf.GetNeededFiles())
	{
		m_penddingFileToLoad.push(file);
	}

	LoadDependencies();

	return false;
}

void CPestModuleLoader::LoadDependencies()
{
	while (m_penddingFileToLoad.empty() == false)
	{
		std::string fileToLoad = m_penddingFileToLoad.front();
		m_penddingFileToLoad.pop();
	}
}