#pragma once
#include <string>
#include <queue>

#include "PtElfProcessor.h"

class CPtModuleLoader
{
public:
	typedef int PSET_SYSV_ABI(*InitProc)(size_t argc, void* argv[], int (*PostInit)(size_t argc, void* argv[]));

	CPtModuleLoader(std::string app0Dir);

	void LoadAndParseElf(const std::string& modulePath, const std::string& fileName);
	void AddModuleDependencies(std::string& fileName);
	void LoadModuleDependencies();
	void RelocateNativeModules();
	void PtFlushInstructionCache();
	void InitNativeModules();

	void* GetEbootEntryPoint();
	void* GetEbootProcParam();
private:
	bool IsNativeModuleLoaded(const std::string& fileName);
	bool GetDirFromModuleName(const std::string& moduleName, std::string& outPath);

	std::queue<std::string> m_pendingDependencies;

	uint32_t m_nNativeModules;
	std::vector<CPtNativeModule>m_natveModules;

	std::string m_app0Dir;
};
CPtModuleLoader* GetElfModuleLoder();