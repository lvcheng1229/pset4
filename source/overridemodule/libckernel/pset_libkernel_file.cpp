#include <fcntl.h>
#include "core\PtApplication.h"
#include "pset_libkernel_file.h"

#define SCE_KERNEL_O_RDONLY        O_RDONLY
#define SCE_KERNEL_O_WRONLY        O_WRONLY 
#define SCE_KERNEL_O_RDWR          O_RDWR
#define SCE_KERNEL_O_NONBLOCK      O_NONBLOCK
#define SCE_KERNEL_O_APPEND        O_APPEND
#define SCE_KERNEL_O_CREAT         O_CREAT
#define SCE_KERNEL_O_TRUNC         O_TRUNC
#define SCE_KERNEL_O_EXCL          O_EXCL
#define SCE_KERNEL_O_DIRECT        O_DIRECT
#define SCE_KERNEL_O_FSYNC         O_FSYNC
#define SCE_KERNEL_O_SYNC          O_SYNC
#define SCE_KERNEL_O_DSYNC         O_DSYNC
#define SCE_KERNEL_O_DIRECTORY     O_DIRECTORY

static std::string PathCast(const char* path)
{
	std::string app0Dir = GetPtApplication()->GetApp0Dir();
	std::string unMappedPath = path;

	std::string retString = unMappedPath;
	
	if (unMappedPath.find("/app0") != std::string::npos)
	{
		retString.replace(0, sizeof("/app0"), app0Dir.c_str());
	}
	else if (unMappedPath.find("/app1") != std::string::npos)
	{
		retString.replace(0, sizeof("/app1"), app0Dir.c_str());
	}
	else if (unMappedPath.find("/savedata") != std::string::npos )
	{
		retString.replace(0, sizeof("/savedata"), PSET_SAVFE_DIR);
	}
	else
	{
		PSET_LOG_ERROR(std::format("PathCast Failed: file path:{},mapped path:{}", path, retString.c_str()));
		retString = "";
	}
	PSET_LOG_INFO(std::format("PathCast: file path:{},mapped path:{}", path, retString.c_str()));
	return retString;
}

int PSET_SYSV_ABI Pset_sceKernelOpen(const char* path, int flags, uint16_t mode)
{
	assert(flags  == 0); // temporary code
	assert((flags & SCE_KERNEL_O_TRUNC) == 0);

	int oflag;
	int pmode;
	if (flags & SCE_KERNEL_O_TRUNC)
	{
		oflag = _O_CREAT | _O_TRUNC | _O_BINARY;
		pmode = _S_IREAD | _S_IWRITE;
	}
	else
	{
		oflag = _O_RDONLY | _O_BINARY;
		pmode = _S_IREAD;
	}

	//std::string mappedPath = GetPtApplication()->MapPs4Path(std::string(path));

	std::string mappedPath = PathCast(path);
	if (mappedPath == "")
	{
		return -1;
	}
	else
	{
		int fd = _open(mappedPath.c_str(), oflag, pmode);
		return fd;
	}
	
}

int PSET_SYSV_ABI Pset_sceKernelClose(int fileHandle)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceKernelClose");
	_close(fileHandle);
	return PSET_OK;
}

int64_t PSET_SYSV_ABI Pset_sceKernelLseek(int fileHandle, int64_t offset, int origin)
{
	PSET_LOG_IMPLEMENTED("implemented function: Pset_sceKernelLseek");
	return _lseeki64(fileHandle, offset, origin);
}

int64_t  PSET_SYSV_ABI Pset_sceKernelRead(int fileHandle, void* destBuffer, size_t maxCharCount)
{
	return _read(fileHandle, destBuffer, maxCharCount);
}

int PSET_SYSV_ABI Pset_sceKernelWrite(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceKernelWrite");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceKernelMkdir(char* path, int32_t mode)
{
	//_mkdir(PathCast(std::string(std::string("/savedata") + path).c_str()).c_str());
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceKernelMkdir");
	return PSET_OK;
}