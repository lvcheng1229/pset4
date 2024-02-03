#pragma once
#include "PsetLibraryCommon.h"

typedef unsigned int SceAjmCodecType;
typedef unsigned int SceAjmContextId;
int PSET_SYSV_ABI sceAjmBatchJobControlBufferRa(void);
int PSET_SYSV_ABI sceAjmBatchJobRunSplitBufferRa(void);
int PSET_SYSV_ABI sceAjmBatchStartBuffer(void);
int PSET_SYSV_ABI sceAjmBatchWait(void);
int PSET_SYSV_ABI sceAjmFinalize(PSET_UNUSED const SceAjmContextId uiContext);
int PSET_SYSV_ABI sceAjmInitialize(PSET_UNUSED int64_t iReserved, SceAjmContextId* const pContext);
int PSET_SYSV_ABI sceAjmInstanceCreate(void);
int PSET_SYSV_ABI sceAjmInstanceDestroy(void);
int PSET_SYSV_ABI sceAjmModuleRegister(PSET_UNUSED const SceAjmContextId uiContext, PSET_UNUSED const SceAjmCodecType uiCodec, PSET_UNUSED int64_t iReserved);
int PSET_SYSV_ABI sceAjmModuleUnregister(void);
