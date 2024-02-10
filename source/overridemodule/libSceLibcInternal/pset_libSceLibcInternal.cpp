#include "pset_libSceLibcInternal.h"

int PSET_SYSV_ABI Pset_sceLibcHeapGetTraceInfo(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceLibcHeapGetTraceInfo");
	return PSET_OK;
}

static const SPSET_LIB_EXPORT_SYSMBOL gSymTable_libSceLibcInternalExt_libSceLibcInternal[] =
{
 { 0x356B53375D1C2731,"Pset_sceLibcHeapGetTraceInfo", (void*)Pset_sceLibcHeapGetTraceInfo },
 SPSET_LIB_EXPORT_FUNTCTION_END
};

static const SPSET_EXPORT_LIB gLibTable_libSceLibcInternal[] =
{
 {"libSceLibcInternalExt", gSymTable_libSceLibcInternalExt_libSceLibcInternal },
 SPSET_EXPORT_LIB_END
};

static const SPSET_EXPORT_MODULE gExportModule_libSceLibcInternal("libSceLibcInternal", gLibTable_libSceLibcInternal);