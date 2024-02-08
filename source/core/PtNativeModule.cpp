#include "PtNativeModule.h"

void CPtNativeModule::GetLibAndModuleName(uint16_t libId, uint16_t moduleId, std::string& outlibName, std::string& outModulebName)
{
	outlibName = m_id2LibraryNameMap[libId];
	outModulebName = m_id2ModuleNameMap[moduleId];
}
