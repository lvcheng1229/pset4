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

private:
	void LoadAndInitEbootModule();

	SElfProcessorCfg m_elfProcessorCfg;

	std::string m_app0File;
	std::string m_app0Dir;
};