#include <filesystem>

#include "PsetModuleLoader.h"
#include "../PsetApplication.h"

CPestModuleLoader::CPestModuleLoader(CPsetDynamicLinker* pDynamicLinker, CPsetApplication* psetApp)
{
	m_pDynamicLinker = pDynamicLinker;
	m_psetApp = psetApp;
}

bool CPestModuleLoader::LoadModule(std::string const& fileName, CPsetModule*& pesetModule)
{
	CPsetModule outModule;
	m_psetElf.LoadFromFile(fileName, &outModule);
	m_psetElf.PrepareProgramHeader();
	m_psetElf.PrepreDynamicSegments();
	m_psetElf.MapImageIntoMemory();

	for (auto& file : m_psetElf.GetNeededFiles())
	{
		m_penddingFileToLoad.push(file);
	}

	LoadDependencies();

	return false;
}

bool CPestModuleLoader::GetPathFromModuleName(const std::string& moduleName, std::string& outPath)
{
	const std::string& app0Path = m_psetApp->m_app0Path;

	const std::filesystem::path& serchPath0 = std::filesystem::path(app0Path) / "sce_module" / moduleName;
	if (std::filesystem::exists(serchPath0))
	{
		outPath = serchPath0.string();
		return true;
	}
	
	return false;
}

void CPestModuleLoader::LoadDependencies()
{
	while (m_penddingFileToLoad.empty() == false)
	{
		std::string fileToLoad = m_penddingFileToLoad.front();
		m_penddingFileToLoad.pop();

		if (m_pDynamicLinker->IsOverrideModule(fileToLoad))
		{
			//TODO:
		}

		std::string outPath;
		if (GetPathFromModuleName(fileToLoad, outPath))
		{
			//TODO:
			CPsetModule* tempModule;
			LoadModule(outPath, tempModule);
		}
	}
}