#include <filesystem>
#include "pset_libSceSaveData.h"

#define SCE_SAVE_DATA_ERROR_PARAMETER							  -2137063424
#define SCE_SAVE_DATA_ERROR_NOT_INITIALIZED						  -2137063423
#define SCE_SAVE_DATA_ERROR_OUT_OF_MEMORY						  -2137063422
#define SCE_SAVE_DATA_ERROR_BUSY								  -2137063421
#define SCE_SAVE_DATA_ERROR_NOT_MOUNTED							  -2137063420
#define SCE_SAVE_DATA_ERROR_NO_PERMISSION						  -2137063419
#define SCE_SAVE_DATA_ERROR_FINGERPRINT_MISMATCH				  -2137063418
#define SCE_SAVE_DATA_ERROR_EXISTS								  -2137063417
#define SCE_SAVE_DATA_ERROR_NOT_FOUND							  -2137063416
#define SCE_SAVE_DATA_ERROR_NO_SPACE_FS							  -2137063414
#define SCE_SAVE_DATA_ERROR_INTERNAL							  -2137063413
#define SCE_SAVE_DATA_ERROR_MOUNT_FULL							  -2137063412
#define SCE_SAVE_DATA_ERROR_BAD_MOUNTED							  -2137063411
#define SCE_SAVE_DATA_ERROR_FILE_NOT_FOUND						  -2137063410
#define SCE_SAVE_DATA_ERROR_BROKEN								  -2137063409
#define SCE_SAVE_DATA_ERROR_INVALID_LOGIN_USER					  -2137063407
#define SCE_SAVE_DATA_ERROR_MEMORY_NOT_READY					  -2137063406
#define SCE_SAVE_DATA_ERROR_BACKUP_BUSY							  -2137063405
#define SCE_SAVE_DATA_ERROR_NOT_REGIST_CALLBACK					  -2137063403
#define SCE_SAVE_DATA_ERROR_BUSY_FOR_SAVING						  -2137063402
#define SCE_SAVE_DATA_ERROR_LIMITATION_OVER						  -2137063401
#define SCE_SAVE_DATA_ERROR_EVENT_BUSY							  -2137063400
#define SCE_SAVE_DATA_ERROR_PARAMSFO_TRANSFER_TITLE_ID_NOT_FOUND  -2137063399

#define SCE_SAVE_DATA_MOUNT_MODE_RDONLY	1
#define SCE_SAVE_DATA_MOUNT_MODE_RDWR	2
#define SCE_SAVE_DATA_MOUNT_MODE_CREATE	4
#define SCE_SAVE_DATA_MOUNT_MODE_DESTRUCT_OFF 8
#define SCE_SAVE_DATA_MOUNT_MODE_COPY_ICON	16
#define SCE_SAVE_DATA_MOUNT_MODE_CREATE2 32

struct SMountDir
{
	char data[SCE_SAVE_DATA_DIRNAME_DATA_MAXSIZE];
};

static SMountDir gMountDir[16];

int FetchSaveMount(char* path, char* mountPoint, int mode)
{
	int result = 0;

	if (path == nullptr || mountPoint == nullptr)
	{
		return SCE_SAVE_DATA_ERROR_PARAMETER;
	}

	SMountDir mountDir;
	uint32_t indexZero = 0;
	for (uint32_t index = 0; index < SCE_SAVE_DATA_DIRNAME_DATA_MAXSIZE; index++)
	{
		indexZero = index;
		if (path[index] == '0')
		{
			break;
		}
		mountDir.data[index] = path[index];
	}

	if (indexZero == 0)
	{
		return SCE_SAVE_DATA_ERROR_PARAMETER;
	}

	std::string savePath = std::string(PSET_SAVFE_DIR) + "/" + std::string(mountDir.data);

	if ((mode & SCE_SAVE_DATA_MOUNT_MODE_CREATE2) != 0)
	{
		assert(false);//unimplemented
	}
	else if ((mode & SCE_SAVE_DATA_MOUNT_MODE_CREATE) != 0)
	{
		assert(false);//unimplemented
	}
	else
	{
		if (!std::filesystem::exists(savePath))
		{
			return SCE_SAVE_DATA_ERROR_NOT_FOUND;
		}
	}

	int32_t m = -1;
	for (uint32_t index = 0; index < 16; index++)
	{
		if (gMountDir[index].data[0] == 0)
		{
			if (m == -1)
			{
				m = index;
				break;
			}
		}
		else
		{
			if (gMountDir[index].data == mountDir.data)
			{
				return SCE_SAVE_DATA_ERROR_BUSY;
			}
		}
	}

	if (m != -1)
	{
		assert(false);
		gMountDir[m] = mountDir;
		std::string mountPointRes("/savedata/");
		mountPointRes += mountDir.data;
		mountPointRes += +"Save";
		memcpy(mountPoint, mountPointRes.c_str(), mountPointRes.length());
	}
	else
	{
		return SCE_SAVE_DATA_ERROR_MOUNT_FULL;
	}
	return result;
}

int PSET_SYSV_ABI Pset_sceSaveDataMount2(SSceSaveDataMount2* pMount, SSceSaveDataMountResult* pMountResult)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSaveDataMount2");
	PSET_LOG_ERROR("unimplemented function: Pset_sceSaveDataMount2");

	if (pMount == nullptr || pMountResult == nullptr)
	{
		return SCE_SAVE_DATA_ERROR_PARAMETER;
	}

	*pMountResult = SSceSaveDataMountResult();

	int result = FetchSaveMount(
		pMount->pSceSaveDataDirName->data,
		(char*)&pMountResult->sceSaveDataMountPoint,
		pMount->mountMode);

	if (result == 0 && ((pMount->mountMode & (SCE_SAVE_DATA_MOUNT_MODE_CREATE | SCE_SAVE_DATA_MOUNT_MODE_CREATE2)) != 0))
	{
		pMountResult->mountStatus = SCE_SAVE_DATA_MOUNT_STATUS_CREATED;
	}

	return PSET_OK;
}