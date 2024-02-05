#pragma once
#include "PsetLibraryCommon.h" 
int PSET_SYSV_ABI Pset_sceMsgDialogClose(void);
int PSET_SYSV_ABI Pset_sceMsgDialogGetResult(void);
int PSET_SYSV_ABI Pset_sceMsgDialogGetStatus(void);
int PSET_SYSV_ABI Pset_sceMsgDialogInitialize(void);
int PSET_SYSV_ABI Pset_sceMsgDialogOpen(void);
int PSET_SYSV_ABI Pset_sceMsgDialogProgressBarInc(void);
int PSET_SYSV_ABI Pset_sceMsgDialogProgressBarSetMsg(void);
int PSET_SYSV_ABI Pset_sceMsgDialogProgressBarSetValue(void);
int PSET_SYSV_ABI Pset_sceMsgDialogTerminate(void);
int PSET_SYSV_ABI Pset_sceMsgDialogUpdateStatus(void);
