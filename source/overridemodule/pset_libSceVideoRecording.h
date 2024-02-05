#pragma once
#include "PsetLibraryCommon.h" 
int PSET_SYSV_ABI Pset_sceVideoRecordingClose(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingCopyBGRAtoNV12(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingGetInfo(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingGetStatus(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingInit(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingOpen(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingOpen2(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingQueryHddSize2(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingQueryMemSize(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingQueryMemSize2(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingSetInfo(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingStart(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingStop(void);
int PSET_SYSV_ABI Pset_sceVideoRecordingTerm(void);
