#pragma once
#include "include\pthread.h"
#include "overridemodule\PsetLibraryCommon.h" 

struct spset_pthread_mutex_t
{
	uint32_t        dummy[256];
	pthread_mutex_t handle;
};

struct spset_pthread_attr_t
{
	uint32_t       dummy[256];
	pthread_attr_t handle;
};

typedef enum EScePthreadMutextype
{
	SCE_PTHREAD_MUTEX_ERRORCHECK = 1,
	SCE_PTHREAD_MUTEX_RECURSIVE = 2,
	SCE_PTHREAD_MUTEX_NORMAL = 3,
	SCE_PTHREAD_MUTEX_ADAPTIVE_NP = 4,
	SCE_PTHREAD_MUTEX_TYPE_MAX
} ScePthreadMutextype;

int PSET_SYSV_ABI Pset_scePthreadMutexattrSettype(pthread_mutexattr_t* attr, int type);
int PSET_SYSV_ABI Pset_scePthreadMutexattrInit(pthread_mutexattr_t* pAttr);
int PSET_SYSV_ABI Pset_scePthreadMutexInit(spset_pthread_mutex_t** m, const pthread_mutexattr_t* a, const char* name);
int PSET_SYSV_ABI Pset_scePthreadMutexattrDestroy(pthread_mutexattr_t* attr);

int PSET_SYSV_ABI Pset_scePthreadAttrInit(spset_pthread_attr_t** attr);
pthread_t PSET_SYSV_ABI Pset_scePthreadSelf(void);

int PSET_SYSV_ABI Pset_scePthreadMutexLock(spset_pthread_mutex_t** mutex);
int PSET_SYSV_ABI Pset_scePthreadMutexUnlock(spset_pthread_mutex_t** mutex);

int PSET_SYSV_ABI Pset_scePthreadAttrGet(pthread_t thread, spset_pthread_attr_t** attr);
int PSET_SYSV_ABI Pset_scePthreadAttrGetaffinity(pthread_t thread, uint64_t* kernelCpuMask);
int PSET_SYSV_ABI Pset_scePthreadAttrDestroy(spset_pthread_attr_t** attr);

int PSET_SYSV_ABI Pset_pthread_mutex_lock(pthread_mutex_t* mtx);
int PSET_SYSV_ABI Pset_pthread_mutex_unlock(pthread_mutex_t* mtx);

int PSET_SYSV_ABI Pset_pthread_cond_broadcast(pthread_cond_t** pPcond);

int PSET_SYSV_ABI Pset_pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);
int PSET_SYSV_ABI Pset_scePthreadMutexDestroy(spset_pthread_mutex_t** pMutex);






