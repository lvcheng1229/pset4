#pragma once
#include "PsetLibraryCommon.h" 
int PSET_SYSV_ABI Pset_sceNpAuthAbortRequest(void);
int PSET_SYSV_ABI Pset_sceNpAuthCreateAsyncRequest(void);
int PSET_SYSV_ABI Pset_sceNpAuthCreateRequest(void);
int PSET_SYSV_ABI Pset_sceNpAuthDeleteRequest(void);
int PSET_SYSV_ABI Pset_sceNpAuthGetAuthorizationCode(void);
int PSET_SYSV_ABI Pset_sceNpAuthGetAuthorizationCodeA(void);
int PSET_SYSV_ABI Pset_sceNpAuthGetIdToken(void);
int PSET_SYSV_ABI Pset_sceNpAuthGetIdTokenA(void);
int PSET_SYSV_ABI Pset_sceNpAuthPollAsync(void);
int PSET_SYSV_ABI Pset_sceNpAuthSetTimeout(void);
int PSET_SYSV_ABI Pset_sceNpAuthWaitAsync(void);
