#pragma once
#include <string>
#include "Elf.h"
class CProgram
{
public:
	std::string m_app0File;
	std::string m_app0Path;
	void LoadProgram();
private:
	CElf m_elf;
};