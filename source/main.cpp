#include <cxxopts.hpp>
#include <iostream>

#include "core/Log.h"
#include "core/PsetThread.h"

#include "PsetApplication.h"
#include "core/PsetModule.h"
#include "core/PsetModuleLoader.h"

cxxopts::ParseResult ParseCmd(int argc, char* argv[])
{
	cxxopts::Options opts("PSET4", "Open Source PlayStation 4 Emulator");
	opts.allow_unrecognised_options();
	opts.add_options()
		("e,eboot", "the file path of elf or self(eboot.bin)", cxxopts::value<std::string>())
		("f,folder", "the folder of app (/app0) [Optional]", cxxopts::value<std::string>())
		("h,help", "Print help message.");

	const uint32_t argCount = argc;

	auto optResult = opts.parse(argc, argv);
	if (optResult.count("h") || argCount < 2)
	{
		PSET_LOG_ERROR(opts.help());
		exit(-1);
	}

	return optResult;
}

int main(int argc, char* argv[])
{
	cxxopts::ParseResult parseResult = ParseCmd(argc, argv);
	PSET_EXIT_AND_LOG_IF(parseResult["e"].count() <= 0, "couldn't find app0 file");

	//CPsetApplication psetApp;
	//psetApp.m_app0File = parseResult["e"].as<std::string>();
	//if (parseResult["f"].count() > 0)
	//{
	//	psetApp.m_app0Path = parseResult["f"].as<std::string>();
	//}
	//else
	//{
	//	size_t posDir = psetApp.m_app0File.find_last_of("/\\");
	//	psetApp.m_app0Path = psetApp.m_app0File.substr(0, posDir);
	//}
	//psetApp.LoadProgram();

	CPsetModule* psetModule = nullptr;
	CPestModuleLoader psetModuleLoader;
	psetModuleLoader.LoadModule(parseResult["e"].as<std::string>(), psetModule);

	CPsetThread psetThread(psetModule->GetEntryPoint());
	psetThread.Run();

    return 1;
}