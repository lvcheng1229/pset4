#include "PsetLibraryCommon.h"
#include "core/PsetDynamicLinker.h"

SPSET_EXPORT_MODULE::SPSET_EXPORT_MODULE(const char* moduleName, const SPSET_EXPORT_LIB* pLibraries)
{
	GetDynamicLinker()->InitializeOverrideModule(moduleName, pLibraries);
}
