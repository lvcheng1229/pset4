#pragma once
#include "PsetLibraryCommon.h" 
int PSET_SYSV_ABI Pset_sceSysmoduleGetModuleHandleInternal(void);
int PSET_SYSV_ABI Pset_sceSysmoduleIsCalledFromSysModule(void);
int PSET_SYSV_ABI Pset_sceSysmoduleIsCameraPreloaded(void);
int PSET_SYSV_ABI Pset_sceSysmoduleIsLoaded(void);
int PSET_SYSV_ABI Pset_sceSysmoduleIsLoadedInternal(void);
int PSET_SYSV_ABI Pset_sceSysmoduleLoadModule(void);
int PSET_SYSV_ABI Pset_sceSysmoduleLoadModuleByNameInternal(void);
int PSET_SYSV_ABI Pset_sceSysmoduleLoadModuleInternal(void);
int PSET_SYSV_ABI Pset_sceSysmoduleLoadModuleInternalWithArg(void);
int PSET_SYSV_ABI Pset_sceSysmoduleMapLibcForLibkernel(void);
int PSET_SYSV_ABI Pset_sceSysmodulePreloadModuleForLibkernel(void);
int PSET_SYSV_ABI Pset_sceSysmoduleUnloadModule(void);
int PSET_SYSV_ABI Pset_sceSysmoduleUnloadModuleByNameInternal(void);
int PSET_SYSV_ABI Pset_sceSysmoduleUnloadModuleInternal(void);
int PSET_SYSV_ABI Pset_sceSysmoduleUnloadModuleInternalWithArg(void);
