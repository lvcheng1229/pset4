#include "PsetModule.h"

void* CPsetModule::GetEntryPoint() const
{
	return m_moduleInfo.pEntryPoint;
}