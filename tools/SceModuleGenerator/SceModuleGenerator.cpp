#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

#include "json.hpp"
#include "cxxopts.hpp"

cxxopts::ParseResult ParseCmd(int argc, char* argv[])
{
	cxxopts::Options opts("PSET4", "Open Source PlayStation 4 Emulator");
	opts.allow_unrecognised_options();
	opts.add_options()
		("i,InputModulesPath", "the path of text file storing the names of input modules e.g. ../input.txt", cxxopts::value<std::string>())
		("h,help", "Print help message.");

	const uint32_t argCount = argc;

	auto optResult = opts.parse(argc, argv);
	if (optResult.count("h") || argCount < 2)
	{
		std::cout << opts.help();
		exit(-1);
	}

	return optResult;
}

std::vector<std::string> GetImportModuleNames(std::string filePath)
{
	std::vector<std::string> output;
	std::ifstream file(filePath);
	const int maxLength = 100;
	char line[maxLength];
	while (true)
	{
		file.getline(line, maxLength);
		const std::string outStr(line);
		if (outStr == "PsetEnd")
		{
			break;
		}
		output.push_back(outStr);
	}
	file.close();
	return output;
};

struct SNidFuncTable
{
	std::string m_nid;
	std::string m_funcName;
};

void GenerateCppFile(const std::unordered_map<std::string, std::vector<SNidFuncTable>>& libName2FuncTableMap, const std::string& moduleName)
{
	;
	// Generate Header
	std::string headerName("pset_" + moduleName + ".h");
	std::string headerCode("#pragma once\n#include \"PsetLibraryCommon.h\" \n");
	for (const auto& lib : libName2FuncTableMap)
	{
		for (const auto& func : lib.second)
		{
			const std::string funcDeclare("int PSET_SYSV_ABI "+ func.m_funcName + "(void);\n");
			headerCode += funcDeclare;
		}
	}
	
	std::ofstream headerFile(SCE_MOUDLE_OUTPUT_DIR + headerName);
	headerFile << headerCode;
	headerFile.close();

	std::string sourceName("pset_" + moduleName + ".cpp");
	std::string sourceCode("//Generated By SceModuleGenerator\n#include \"" + headerName + "\"\n\n\n");
	
	// function implementation
	for (const auto& lib : libName2FuncTableMap)
	{
		for (const auto& func : lib.second)
		{
			const std::string funcDeclare(
				"int PSET_SYSV_ABI " + func.m_funcName + "(void)\n" + 
				"{\n" +
				"	PSET_LOG_UNIMPLEMENTED(\"unimplemented function: " + func.m_funcName + "\");\n"
				"	return PSET_OK;\n"+
				"}\n\n"
			);
			sourceCode += funcDeclare;
		}
	}

	// export table
	for (const auto& lib : libName2FuncTableMap)
	{
		sourceCode += "static const SPSET_LIB_EXPORT_SYSMBOL gSymTable_" + lib.first + "_" + moduleName + "[] =\n";
		sourceCode += "{\n";
		for (const auto& func : lib.second)
		{
			sourceCode += " { 0x" + func.m_nid + ", (void*)" + func.m_funcName + " },\n";
		}
		sourceCode += " SPSET_LIB_EXPORT_FUNTCTION_END\n";
		sourceCode += "};\n\n";
	}

	sourceCode += "static const SPSET_EXPORT_LIB gLibTable_" + moduleName + "[] =\n";
	sourceCode += "{\n";
	for (const auto& lib : libName2FuncTableMap)
	{
		sourceCode += " {\"" + lib.first + "\", gSymTable_" + lib.first +"_" + moduleName + " },\n";
	}
	sourceCode += " SPSET_EXPORT_LIB_END\n";
	sourceCode += "};\n\n";

	sourceCode += "static const SPSET_EXPORT_MODULE gExportModule_" + moduleName;
	sourceCode += "(\"" + moduleName + "\", gLibTable_" + moduleName + ");";

	std::ofstream sourceFile(SCE_MOUDLE_OUTPUT_DIR + sourceName);
	sourceFile << sourceCode;
	sourceFile.close();
}

void GetImportModuleSymbols(std::vector<std::string>& importModules)
{
	for (uint32_t index = 0; index < importModules.size(); index++)
	{
		const std::string& moduleName = importModules[index] + ".sprx.json";
		std::ifstream file(SCE_LIBS_PATH + moduleName);
		if (std::filesystem::exists(SCE_LIBS_PATH + moduleName))
		{
			nlohmann::json jsonData = nlohmann::json::parse(file);
			bool bFound = false;
			for (auto& modules : jsonData["modules"])
			{
				std::string subModuleName = modules.at("name").get<std::string>();
				if (subModuleName == importModules[index])
				{
					std::unordered_map<std::string, std::vector<SNidFuncTable>> libName2FuncTableMap;
					for (auto& libraries : modules["libraries"])
					{
						std::string libName = libraries["name"].get<std::string>();
						if (libName2FuncTableMap.find(libName) == libName2FuncTableMap.end())
						{
							libName2FuncTableMap.emplace(std::make_pair(libName, std::vector<SNidFuncTable>()));
						}

						for (auto& symbols : libraries["symbols"])
						{
							std::string hexid = symbols["hex_id"].get<std::string>();
							std::string symName;
							if (symbols["name"] != nullptr)
							{
								symName = "Pset_" + symbols["name"].get<std::string>();
							}
							else
							{
								symName = "NullNameFunc" + hexid;
							}

							bool bExsit = false;
							for (const auto& lib : libName2FuncTableMap)
							{
								for (const auto& func : lib.second)
								{
									if (func.m_nid == hexid)
									{
										bExsit = true;
									}
								}
							}

							if (bExsit == false)
							{
								libName2FuncTableMap[libName].push_back(SNidFuncTable{ hexid,symName });
							}
						}
					}

					GenerateCppFile(libName2FuncTableMap, subModuleName);
					bFound = true;
				}
			}

			if (!bFound)
			{
				std::cout << "coudnt found module:" << moduleName << std::endl;
			}
		}
		else
		{
			std::cout << "coudnt found module:" << moduleName << std::endl;
		}
		
		//coudnt found module:libGameCustomDataDialog.sprx.json
		//coudnt found module : libSceNpScore.sprx.json
		//coudnt found module : libSceNpToolkit2.sprx.json

	}
}

int main(int argc, char* argv[])
{
	std::filesystem::path outputFolder(SCE_MOUDLE_OUTPUT_DIR);
	if (!std::filesystem::exists(outputFolder))
	{
		std::filesystem::create_directory(outputFolder);
	}

	cxxopts::ParseResult parseResult = ParseCmd(argc, argv);
	std::vector<std::string> importModules = GetImportModuleNames(parseResult["i"].as<std::string>());

	//std::vector<std::string> testFile;
	//testFile.push_back(std::string("libSceAudio3d"));
	GetImportModuleSymbols(importModules);
	std::cout << "save output file to " << SCE_MOUDLE_OUTPUT_DIR << std::endl;
	return 1;
}