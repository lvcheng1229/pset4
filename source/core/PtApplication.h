#pragma once
#include <string>
#include "PtModuleLoader.h"
class CPtApplication
{
public:
	~CPtApplication();

	void ParseCommnadLine(int argc, char* argv[]);
	void Init();
	void Run();

	//std::string MapPs4Path(const std::string& ps4Path);
	std::string GetApp0Dir() { return m_app0Dir; }
private:
	void LoadAndInitEbootModule();

	SElfProcessorCfg m_elfProcessorCfg;

	std::string m_app0File;
	std::string m_app0Dir;
};

CPtApplication* GetPtApplication();