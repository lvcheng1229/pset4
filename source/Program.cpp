#include "Elf.h"
#include "Program.h"

void CProgram::LoadProgram()
{
	m_elf = CElf::LoadElf(m_app0File);
}
