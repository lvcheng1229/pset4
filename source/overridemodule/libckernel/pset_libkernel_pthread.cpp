#include "pset_kernel_error.h"
#include "pset_libkernel_pthread.h"

static int pthreadErrorToSceError(int perror)
{
	int sceError = SCE_KERNEL_ERROR_UNKNOWN;

	switch (perror)
	{
	case 0:
		sceError = PSET_OK;
		break;
	case EPERM:
		sceError = SCE_KERNEL_ERROR_EPERM;
		break;
	case ENOENT:
		sceError = SCE_KERNEL_ERROR_ENOENT;
		break;
	case ESRCH:
		sceError = SCE_KERNEL_ERROR_ESRCH;
		break;
	case EINTR:
		sceError = SCE_KERNEL_ERROR_EINTR;
		break;
	case EIO:
		sceError = SCE_KERNEL_ERROR_EIO;
		break;
	case ENXIO:
		sceError = SCE_KERNEL_ERROR_ENXIO;
		break;
	case E2BIG:
		sceError = SCE_KERNEL_ERROR_E2BIG;
		break;
	case ENOEXEC:
		sceError = SCE_KERNEL_ERROR_ENOEXEC;
		break;
	case EBADF:
		sceError = SCE_KERNEL_ERROR_EBADF;
		break;
	case ECHILD:
		sceError = SCE_KERNEL_ERROR_ECHILD;
		break;
	case EAGAIN:
		sceError = SCE_KERNEL_ERROR_EAGAIN;
		break;
	case ENOMEM:
		sceError = SCE_KERNEL_ERROR_ENOMEM;
		break;
	case EACCES:
		sceError = SCE_KERNEL_ERROR_EACCES;
		break;
	case EFAULT:
		sceError = SCE_KERNEL_ERROR_EFAULT;
		break;
	case EBUSY:
		sceError = SCE_KERNEL_ERROR_EBUSY;
		break;
	case EEXIST:
		sceError = SCE_KERNEL_ERROR_EEXIST;
		break;
	case EXDEV:
		sceError = SCE_KERNEL_ERROR_EXDEV;
		break;
	case ENODEV:
		sceError = SCE_KERNEL_ERROR_ENODEV;
		break;
	case ENOTDIR:
		sceError = SCE_KERNEL_ERROR_ENOTDIR;
		break;
	case EISDIR:
		sceError = SCE_KERNEL_ERROR_EISDIR;
		break;
	case EINVAL:
		sceError = SCE_KERNEL_ERROR_EINVAL;
		break;
	case ENFILE:
		sceError = SCE_KERNEL_ERROR_ENFILE;
		break;
	case EMFILE:
		sceError = SCE_KERNEL_ERROR_EMFILE;
		break;
	case ENOTTY:
		sceError = SCE_KERNEL_ERROR_ENOTTY;
		break;
	case EFBIG:
		sceError = SCE_KERNEL_ERROR_EFBIG;
		break;
	case ENOSPC:
		sceError = SCE_KERNEL_ERROR_ENOSPC;
		break;
	case ESPIPE:
		sceError = SCE_KERNEL_ERROR_ESPIPE;
		break;
	case EROFS:
		sceError = SCE_KERNEL_ERROR_EROFS;
		break;
	case EMLINK:
		sceError = SCE_KERNEL_ERROR_EMLINK;
		break;
	case EPIPE:
		sceError = SCE_KERNEL_ERROR_EPIPE;
		break;
	case EDOM:
		sceError = SCE_KERNEL_ERROR_EDOM;
		break;
	case EDEADLK:
		sceError = SCE_KERNEL_ERROR_EDEADLK;
		break;
	case ENAMETOOLONG:
		sceError = SCE_KERNEL_ERROR_ENAMETOOLONG;
		break;
	case ENOLCK:
		sceError = SCE_KERNEL_ERROR_ENOLCK;
		break;
	case ENOSYS:
		sceError = SCE_KERNEL_ERROR_ENOSYS;
		break;
	case ENOTEMPTY:
		sceError = SCE_KERNEL_ERROR_ENOTEMPTY;
		break;
	default:
	{
		sceError = perror;
	}
	break;
	}

	return sceError;
}

inline int ConvertSceDetachStateToPthreadState(int oldState)
{
	int newState = -1;
	switch (oldState)
	{
	case SCE_PTHREAD_CREATE_DETACHED:
		newState = PTHREAD_CREATE_DETACHED;
		break;
	case SCE_PTHREAD_CREATE_JOINABLE:
		newState = PTHREAD_CREATE_JOINABLE;
		break;
	default:
		assert(false);
		break;
	}
	return newState;
}

int PSET_SYSV_ABI Pset_scePthreadMutexattrInit(pthread_mutexattr_t* pAttr)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadMutexattrInit");
	int err = pthread_mutexattr_init(pAttr);
	assert(err == 0);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadMutexInit(spset_pthread_mutex_t** m, const pthread_mutexattr_t* a, const char* name)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadMutexInit,Name:" + std::string(name));
	spset_pthread_mutex_t* object = (spset_pthread_mutex_t*)calloc(1, sizeof(spset_pthread_mutex_t));
	int err = 0;
	if (a == nullptr)
	{
		// If attr is nullptr then default will be used which is PTHREAD_MUTEX_ERRORCHECK on PS4
		// so we make sure we set PTHREAD_MUTEX_ERRORCHECK here to match behavior.
		pthread_mutexattr_t errorCheckMutexAttr;
		pthread_mutexattr_init(&errorCheckMutexAttr);
		pthread_mutexattr_settype(&errorCheckMutexAttr, PTHREAD_MUTEX_ERRORCHECK);
		err = pthread_mutex_init(&object->handle, &errorCheckMutexAttr);
		pthread_mutexattr_destroy(&errorCheckMutexAttr);
	}
	else
	{
		err = pthread_mutex_init(&object->handle, a);
	}
	*m = object;
	assert(err == 0);
	return PSET_OK;
}

int sceMutexAttrTypeToPthreadType(int sceType)
{
	int pthreadType = -1;
	switch (sceType)
	{
	case EScePthreadMutextype::SCE_PTHREAD_MUTEX_ERRORCHECK:
		pthreadType = PTHREAD_MUTEX_ERRORCHECK;
		break;
	case SCE_PTHREAD_MUTEX_RECURSIVE:
		pthreadType = PTHREAD_MUTEX_RECURSIVE;
		break;
	case SCE_PTHREAD_MUTEX_NORMAL:
		pthreadType = PTHREAD_MUTEX_NORMAL;
		break;
	case SCE_PTHREAD_MUTEX_ADAPTIVE_NP:
		pthreadType = PTHREAD_MUTEX_ADAPTIVE_NP;
		break;
	default:
		assert(false);
		break;
	}
	return pthreadType;
}

int PSET_SYSV_ABI Pset_scePthreadMutexattrSettype(pthread_mutexattr_t* attr, int type)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadMutexattrSettype");
	int ptype = sceMutexAttrTypeToPthreadType(type);
	int err = pthread_mutexattr_settype(attr, ptype);
	assert(err == 0);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadMutexattrDestroy(pthread_mutexattr_t* attr)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadMutexattrDestroy");
	int err = pthread_mutexattr_destroy(attr);
	assert(err == 0);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadAttrInit(spset_pthread_attr_t** attr)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadAttrInit");
	spset_pthread_attr_t* object = (spset_pthread_attr_t*)calloc(1, sizeof(spset_pthread_attr_t));
	int                 err = pthread_attr_init(&object->handle);
	*attr = object;
	assert(err == 0);
	return PSET_OK;
}

pthread_t PSET_SYSV_ABI Pset_scePthreadSelf(void)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadSelf");
	pthread_t pt = pthread_self();
	return pt;
}

int PSET_SYSV_ABI Pset_scePthreadMutexLock(spset_pthread_mutex_t** mutex)
{
	//PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadMutexLock");
	int err = Pset_pthread_mutex_lock(&((*mutex)->handle));
	assert(err == 0);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadMutexUnlock(spset_pthread_mutex_t** mutex)
{
	//PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadMutexUnlock");
	int err = Pset_pthread_mutex_unlock(&((*mutex)->handle));
	assert(err == 0);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadAttrGet(pthread_t thread, spset_pthread_attr_t** attr)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadAttrGet");
	pthread_attr_init(&((*attr)->handle));
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadAttrGetaffinity(pthread_t thread, uint64_t* kernelCpuMask)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadAttrGetaffinity");
	*kernelCpuMask = 0;
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadAttrDestroy(spset_pthread_attr_t** attr)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadAttrDestroy");
	int err = pthread_attr_destroy(&((*attr)->handle));
	free(*attr);
	assert(err == 0);
	return PSET_OK;
}


int PSET_SYSV_ABI Pset_pthread_mutex_lock(pthread_mutex_t* mtx)
{
	//PSET_LOG_IMPLEMENTED(std::format("implemented function: Pset_pthread_mutex_lock pointer:{}", uint64_t(mtx)));
	int err = pthread_mutex_trylock(mtx);
	if (err == EBUSY)
	{
		//PSET_LOG_ERROR("Pset_pthread_mutex_lock::Failed To Lock");
	}
	//assert(err == 0);
	return pthreadErrorToSceError(err);
}

int PSET_SYSV_ABI Pset_pthread_mutex_unlock(pthread_mutex_t* mtx)
{
	//PSET_LOG_IMPLEMENTED(std::format("implemented function: Pset_pthread_mutex_unlock pointer:{}", uint64_t(mtx)));
	int err = pthread_mutex_unlock(mtx);
	assert(err == 0);;
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_pthread_cond_broadcast(pthread_cond_t* pCond)
{
	//TODO:

	assert(pCond != nullptr);
	if (*pCond == 0)
	{
		return PSET_OK;
	}
	
	PSET_LOG_IMPLEMENTED("implemented function: Pset_pthread_cond_broadcast");
	int err = pthread_cond_broadcast(pCond);
	//assert(err == 0);
	//return pthreadErrorToSceError(err);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_pthread_mutex_init");
	int err = pthread_mutex_init(mutex, attr);
	assert(err == 0);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadMutexDestroy(spset_pthread_mutex_t** pMutex)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadMutexDestroy");
	int err = pthread_mutex_destroy(&((*pMutex)->handle));
	assert(err == 0);
	free(*pMutex);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadAttrSetdetachstate(spset_pthread_attr_t** attr, int state)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_scePthreadAttrSetdetachstate");
	int pthreadState = ConvertSceDetachStateToPthreadState(state);
	int err = pthread_attr_setdetachstate(&((*attr)->handle), pthreadState);
	assert(err == 0);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_scePthreadSetaffinity(pthread_t thread, const uint64_t mask)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_scePthreadSetaffinity");
	return PSET_OK;
}

typedef void* (PSET_SYSV_ABI* PFUNC_PS4_THREAD_ENTRY)(void*);
struct SCE_THREAD_PARAM
{
	void* entry;
	void* arg;
};
void* newThreadWrapper(void* arg)
{
	SCE_THREAD_PARAM* param = (SCE_THREAD_PARAM*)arg;
	PFUNC_PS4_THREAD_ENTRY pSceEntry = (PFUNC_PS4_THREAD_ENTRY)param->entry;
	return  pSceEntry(param->arg);
}

int PSET_SYSV_ABI Pset_scePthreadCreate(pthread_t* pthread, spset_pthread_attr_t** attr, void* (PSET_SYSV_ABI*entry) (void*), void* arg, const char* name)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_scePthreadCreate:Name" + std::string(name));
	SCE_THREAD_PARAM* param = new SCE_THREAD_PARAM();
	param->entry = (void*)entry;
	param->arg = arg;
	int err = pthread_create(pthread, attr ? &((*attr)->handle) : nullptr, newThreadWrapper, param);
	assert(err == 0);
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceKernelUsleep(uint32_t ms)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceKernelUsleep");
	std::this_thread::sleep_for(std::chrono::microseconds(ms));
	return PSET_OK;
}

int PSET_SYSV_ABI Pset__nanosleep(const struct timespec* request, struct timespec* remain)
{
	return nanosleep(request, remain);
}

int PSET_SYSV_ABI Pset_nanosleep(const struct timespec* request, struct timespec* remain)
{
	return nanosleep(request, remain);
}