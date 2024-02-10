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
	GetElfProcessor()->AddModuleDependencies();
}

void CPtModuleLoader::AddModuleDependencies(std::string& fileName)
{
	if (GetPtDynamicLinker()->IsNativeModule(fileName) && !IsNativeModuleLoaded(fileName))
	{
		m_pendingDependencies.push(fileName);
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

void CPtModuleLoader::PtFlushInstructionCache()
{
	for (uint32_t index = 0; index < m_nNativeModules; index++)
	{
		FlushInstructionCache(GetCurrentProcess(), m_natveModules[index].m_moduleInfo.m_mappedMemory.m_pAddress, m_natveModules[index].m_moduleInfo.m_mappedMemory.m_size);
	}
}

void CPtModuleLoader::InitNativeModules()
{
	for (uint32_t index = 0; index < m_nNativeModules; index++)
	{
		if (m_natveModules[index].m_fileName != "eboot.bin")
		{
			auto initFunc = reinterpret_cast<InitProc>(m_natveModules[index].m_moduleInfo.m_pInitProc);
			initFunc(0, nullptr, nullptr);
		}
	}
}

void* CPtModuleLoader::GetEbootEntryPoint()
{
	return m_natveModules[0].GetModuleInfo().m_pEntryPoint;
}

void* CPtModuleLoader::GetEbootProcParam()
{
	return m_natveModules[0].GetModuleInfo().m_pProcParamProc;
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
