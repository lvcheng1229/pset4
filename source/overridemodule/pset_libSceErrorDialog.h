#pragma once
#include "PsetLibraryCommon.h" 
int PSET_SYSV_ABI Pset_sceErrorDialogClose(void);
int PSET_SYSV_ABI Pset_sceErrorDialogGetStatus(void);
int PSET_SYSV_ABI Pset_sceErrorDialogInitialize(void);
int PSET_SYSV_ABI Pset_sceErrorDialogOpen(void);
int PSET_SYSV_ABI Pset_sceErrorDialogOpenDetail(void);
int PSET_SYSV_ABI Pset_sceErrorDialogOpenWithReport(void);
int PSET_SYSV_ABI Pset_sceErrorDialogTerminate(void);
int PSET_SYSV_ABI Pset_sceErrorDialogUpdateStatus(void);
