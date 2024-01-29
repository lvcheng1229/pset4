#include "core/Log.h"
#include "Elf.h"

CElf CElf::LoadElf(const std::string& elfPath)
{
	FILE* elfFile = fopen(elfPath.c_str(), "rb");
	
	PSET_EXIT_AND_LOG_IF(elfFile == nullptr, "couldn't open app");

	return CElf();
}