#pragma once
#include <string>
#include <queue>

#include "PtElfProcessor.h"

class CPtModuleLoader
{
public:
	CPtModuleLoader(std::string app0Dir);

	void LoadAndParseElf(const std::string& modulePath, const std::string& fileName);
	void AddModuleDependencies();
	void LoadModuleDependencies();
	void RelocateNativeModules();
	void InitNativeModules();

	void* GetEbootEntryPoint();
private:
	bool IsNativeModuleLoaded(const std::string& fileName);
	bool GetDirFromModuleName(const std::string& moduleName, std::string& outPath);

	std::queue<std::string> m_pendingDependencies;

	uint32_t m_nNativeModules;
	std::vector<CPtNativeModule>m_natveModules;

	std::string m_app0Dir;
};
CPtModuleLoader* GetElfModuleLoder();