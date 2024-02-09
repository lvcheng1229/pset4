#include <filesystem>
#include "PtModuleLoader.h"
#include "PtDynamicLinker.h"

static constexpr uint32_t defaultMaxNativeModuleNum = 256;

CPtModuleLoader::CPtModuleLoader(std::string app0Dir)
{
	m_app0Dir = app0Dir;
	m_nNativeModules = 0;
	m_natveModules.resize(defaultMaxNativeModuleNum);
}

void CPtModuleLoader::LoadAndParseElf(const std::string& modulePath, const std::string& fileName)
{
	if (IsNativeModuleLoaded(fileName))
	{
		return;
	}

	CPtNativeModule* outModule = &m_natveModules[m_nNativeModules];
	outModule->m_fileName = fileName;
	m_nNativeModules++;

	std::vector<uint8_t> outRawData;
	GetElfProcessor()->SetProcessContext(outModule, modulePath + fileName);
	GetElfProcessor()->LoadRawData(outRawData);
	GetElfProcessor()->MapSelfToElf(outRawData);
	GetElfProcessor()->ParseProgramHeaders();
	GetElfProcessor()->ParseDynamicSegment();
	GetElfProcessor()->MapImageToMemory();
	GetElfProcessor()->ParseAndExportNativeSymbol();
}

void CPtModuleLoader::AddModuleDependencies()
{
	for (uint32_t index = 0; index < m_nNativeModules; index++)
	{
		CPtNativeModule* modu = &m_natveModules[index];
		for (auto& t : modu->m_neededFiles)
		{
			if (!IsNativeModuleLoaded(t) && GetPtDynamicLinker()->IsNativeModule(t))
			{
				m_pendingDependencies.push(t);
			}
			else if (!IsNativeModuleLoaded(t) && (t == "libc.prx" || t == "libSceLibcInternal.sprx" || t == "libSceLibcInternal.prx"))
			{
				m_pendingDependencies.push(t);
			}
		}
	}
}

bool CPtModuleLoader::GetDirFromModuleName(const std::string& moduleName, std::string& outPath)
{
	const std::filesystem::path& serchPath0 = std::filesystem::path(m_app0Dir) / "sce_module" / moduleName;
	if (std::filesystem::exists(serchPath0))
	{
		const std::filesystem::path& outRes = std::filesystem::path(m_app0Dir) / "sce_module/";
		outPath = outRes.string();
		return true;
	}

	return false;
}

void CPtModuleLoader::LoadModuleDependencies()
{
	while (m_pendingDependencies.empty() == false)
	{
		std::string fileToLoad = m_pendingDependencies.front();
		m_pendingDependencies.pop();

		if (IsNativeModuleLoaded(fileToLoad))
		{
			continue;
		}

		std::string outPath;
		if (GetDirFromModuleName(fileToLoad, outPath))
		{
			LoadAndParseElf(outPath, fileToLoad);
		}
	}
}

void CPtModuleLoader::RelocateNativeModules()
{
	GetPtDynamicLinker()->RelocateNativeModules(m_natveModules, m_nNativeModules);
}

void CPtModuleLoader::InitNativeModules()
{
	PSET_LOG_ERROR("TODO:InitNativeModules");
	//assert(false);
}

void* CPtModuleLoader::GetEbootEntryPoint()
{
	return m_natveModules[0].GetModuleInfo().m_pEntryPoint;
}

bool CPtModuleLoader::IsNativeModuleLoaded(const std::string& fileName)
{
	for (uint32_t index = 0; index < m_nNativeModules; index++)
	{
		if (m_natveModules[index].m_fileName == fileName)
		{
			return true;
		}
	}
	return false;
}
