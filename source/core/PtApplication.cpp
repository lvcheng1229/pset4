#include <cxxopts.hpp>
#include <iostream>
#include <filesystem>

#include "PtApplication.h"
#include "PsetThread.h"

static CPtElfProcessor* g_pElfProcessor = nullptr;
static CPtModuleLoader* g_pElfModuleLoader = nullptr;
static CPtApplication* g_pApplication = nullptr;
CPtApplication* GetPtApplication()
{
	if (g_pApplication == nullptr)
	{
		g_pApplication =  new CPtApplication();
	}
	return g_pApplication;
}

CPtElfProcessor* GetElfProcessor()
{
	return g_pElfProcessor;
}

CPtModuleLoader* GetElfModuleLoder()
{
	return g_pElfModuleLoader;
}

CPtApplication::~CPtApplication()
{
	if (g_pElfProcessor)
	{
		delete g_pElfProcessor;
	}

	if (g_pElfModuleLoader)
	{
		delete g_pElfModuleLoader;
	}
}

void CPtApplication::ParseCommnadLine(int argc, char* argv[])
{
	cxxopts::Options opts("PSET4", "Open Source PlayStation 4 Emulator");
	opts.allow_unrecognised_options();
	opts.add_options()
		("e,eboot", "the file path of elf or self(eboot.bin)", cxxopts::value<std::string>())
		("h,help", "Print help message.");

	const uint32_t argCount = argc;

	auto optResult = opts.parse(argc, argv);
	if (optResult.count("h") || argCount < 2)
	{
		PSET_LOG_ERROR(opts.help());
		exit(-1);
	}

	std::string eParam = optResult["e"].as<std::string>();
	size_t posDir = eParam.find_last_of("/\\");
	m_app0File = eParam.substr(posDir+1, eParam.length());
	m_app0Dir = eParam.substr(0, posDir + 1);
}

void CPtApplication::Init()
{
	g_pElfProcessor = new CPtElfProcessor(m_elfProcessorCfg);
	g_pElfModuleLoader = new CPtModuleLoader(m_app0Dir);

	LoadAndInitEbootModule();
}

void CPtApplication::Run()
{
	PSET_LOG_INFO("run application");
	CPsetThread psetThread(GetElfModuleLoder()->GetEbootEntryPoint());
	psetThread.Run();
}

void CPtApplication::LoadAndInitEbootModule()
{
	GetElfModuleLoder()->LoadAndParseElf(m_app0Dir, "eboot.bin");
	GetElfModuleLoder()->LoadModuleDependencies();
	GetElfModuleLoder()->RelocateNativeModules();
	GetElfModuleLoder()->PtFlushInstructionCache();
	GetElfModuleLoder()->InitNativeModules();
	//std::filesystem::create_directory(PSET_SAVFE_DIR);
}


