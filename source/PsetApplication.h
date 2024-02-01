#pragma once
#include <string>
#include "PsetElf.h"
class CPsetApplication
{
public:
	std::string m_app0File;
	std::string m_app0Path;
	void LoadProgram();
private:
	CPsetElf m_program;
};