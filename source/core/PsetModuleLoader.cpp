#include <filesystem>

#include "PsetModuleLoader.h"
#include "PsetApplication.h"

CPestModuleLoader::CPestModuleLoader(CPsetDynamicLinker* pDynamicLinker, CPsetApplication* psetApp)
{
	m_pDynamicLinker = pDynamicLinker;
	m_psetApp = psetApp;
}

bool CPestModuleLoader::LoadModule(const std::string& modulePath, const std::string& moduleName)
{
	CPsetModule* nativeModule = m_pDynamicLinker->AddNativeModule(moduleName);

	bool bEbootModule = moduleName == "eboot.bin";

	m_psetElf.LoadFromFile(modulePath, nativeModule, bEbootModule);
	m_psetElf.PrepareProgramHeader();
	m_psetElf.PrepreDynamicSegments();
	m_psetElf.MapImageIntoMemory();

	if (bEbootModule)
	{
		m_loadedModuleNames.insert(moduleName);

		for (auto& file : m_psetElf.GetNeededFiles())
		{
			size_t pos = file.find('.');
			const std::string moduleNameRemoveStr = file.substr(0, pos);

			auto iter = m_loadedModuleNames.find(file);
			if (iter == m_loadedModuleNames.end() && !m_pDynamicLinker->IsOverrideModule(moduleNameRemoveStr))
			{
				m_penddingFileToLoad.push(file);
			}
		}
		LoadDependencies();
	}


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
			continue;
		}

		std::string outPath;
		if (GetPathFromModuleName(fileToLoad, outPath))
		{
			LoadModule(outPath, fileToLoad);
		}
	}
}