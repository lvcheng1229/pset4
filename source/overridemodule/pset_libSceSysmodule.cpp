//Generated By SceModuleGenerator
#include "pset_libSceSysmodule.h"


int PSET_SYSV_ABI Pset_sceSysmoduleGetModuleHandleInternal(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleGetModuleHandleInternal");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleIsCalledFromSysModule(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleIsCalledFromSysModule");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleIsCameraPreloaded(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleIsCameraPreloaded");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleIsLoaded(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleIsLoaded");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleIsLoadedInternal(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleIsLoadedInternal");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleLoadModule(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleLoadModule");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleLoadModuleByNameInternal(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleLoadModuleByNameInternal");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleLoadModuleInternal(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleLoadModuleInternal");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleLoadModuleInternalWithArg(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleLoadModuleInternalWithArg");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleMapLibcForLibkernel(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleMapLibcForLibkernel");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmodulePreloadModuleForLibkernel(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmodulePreloadModuleForLibkernel");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleUnloadModule(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleUnloadModule");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleUnloadModuleByNameInternal(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleUnloadModuleByNameInternal");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleUnloadModuleInternal(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleUnloadModuleInternal");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSysmoduleUnloadModuleInternalWithArg(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSysmoduleUnloadModuleInternalWithArg");
	return PSET_OK;
}

static const SPSET_LIB_EXPORT_SYSMBOL gSymTable_libSceSysmodule_libSceSysmodule[] =
{
 { 0x0FC72E53877BDB13, (void*)Pset_sceSysmoduleGetModuleHandleInternal },
 { 0x72D7CEEDD43B81E8, (void*)Pset_sceSysmoduleIsCalledFromSysModule },
 { 0x9E8E93DC47E24B71, (void*)Pset_sceSysmoduleIsCameraPreloaded },
 { 0x7CC3F934750E68C9, (void*)Pset_sceSysmoduleIsLoaded },
 { 0xCA714A4396DF1A4B, (void*)Pset_sceSysmoduleIsLoadedInternal },
 { 0x83C70CDFD11467AA, (void*)Pset_sceSysmoduleLoadModule },
 { 0x094F26F90B3E1CDE, (void*)Pset_sceSysmoduleLoadModuleByNameInternal },
 { 0xDFD895E44D47A029, (void*)Pset_sceSysmoduleLoadModuleInternal },
 { 0x847AC6A06A0D7FEB, (void*)Pset_sceSysmoduleLoadModuleInternalWithArg },
 { 0x959E91BD5974BE8D, (void*)Pset_sceSysmoduleMapLibcForLibkernel },
 { 0x0CE3BECEE5B596B1, (void*)Pset_sceSysmodulePreloadModuleForLibkernel },
 { 0x791D9B6450005344, (void*)Pset_sceSysmoduleUnloadModule },
 { 0xBE94C7980E8A9EF8, (void*)Pset_sceSysmoduleUnloadModuleByNameInternal },
 { 0xBD7661AED2719067, (void*)Pset_sceSysmoduleUnloadModuleInternal },
 { 0x68A6BA61F04A66CE, (void*)Pset_sceSysmoduleUnloadModuleInternalWithArg },
 SPSET_LIB_EXPORT_FUNTCTION_END
};

static const SPSET_EXPORT_LIB gLibTable_libSceSysmodule[] =
{
 {"libSceSysmodule", gSymTable_libSceSysmodule_libSceSysmodule },
 SPSET_EXPORT_LIB_END
};

static const SPSET_EXPORT_MODULE gExportModule_libSceSysmodule("libSceSysmodule", gLibTable_libSceSysmodule);