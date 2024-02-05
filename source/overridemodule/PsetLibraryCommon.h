#pragma once
#include <stdint.h>
#include "core/Log.h"
#include "core/Common.h"

struct SPSET_LIB_EXPORT_SYSMBOL
{
	const uint64_t m_nid;
	const void* m_pFunction;
};
#define SPSET_LIB_EXPORT_FUNTCTION_END {0,nullptr}

struct SPSET_EXPORT_LIB
{
	const char* m_libName;
	const SPSET_LIB_EXPORT_SYSMBOL* m_pFunctionEntries;
};
#define SPSET_EXPORT_LIB_END {nullptr,nullptr}

struct SPSET_EXPORT_MODULE
{
	SPSET_EXPORT_MODULE(const char* moduleName, const SPSET_EXPORT_LIB* pLibraries);
	const char* m_moduleName;
	const SPSET_EXPORT_LIB* m_pLibraries;
};