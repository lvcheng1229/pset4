#pragma once
#include "overridemodule/PsetLibraryCommon.h" 


//PSceSaveDataMount2 = ^ SceSaveDataMount2;
//SceSaveDataMount2 = packed record
//userId : Integer;
//align1:Integer;
//dirName:PSceSaveDataDirName;
//blocks:QWORD;
//mountMode:DWORD;
//reserved:array[0..31] of Byte;
//align2:Integer;
//end;

#define SCE_SAVE_DATA_TITLE_ID_DATA_SIZE 10
#define SCE_SAVE_DATA_FINGERPRINT_DATA_SIZE 65

#define SCE_SAVE_DATA_TITLE_MAXSIZE 128
#define SCE_SAVE_DATA_SUBTITLE_MAXSIZE 128
#define SCE_SAVE_DATA_DETAIL_MAXSIZE 1024

#define SCE_SAVE_DATA_DIRNAME_DATA_MAXSIZE 32
#define SCE_SAVE_DATA_MOUNT_POINT_DATA_MAXSIZE  16
#define SCE_SAVE_DATA_MOUNT_STATUS_CREATED 0x00000001

//struct  SSceSaveDataFingerprint
//{
//	uint8_t data[SCE_SAVE_DATA_FINGERPRINT_DATA_SIZE];
//	uint8_t padding[15];
//};

struct SSceSaveDataDirName
{
	char data[SCE_SAVE_DATA_DIRNAME_DATA_MAXSIZE];
};
struct SSceSaveDataMount2
{
	int32_t userId;
	int32_t align1;
	
	SSceSaveDataDirName* pSceSaveDataDirName;

	uint64_t blocks;
	uint64_t mountMode;
	uint8_t reserved[32];
	int32_t align2;
};

struct SSceSaveDataMountPoint
{
	char SceSaveDataMountPoint[SCE_SAVE_DATA_MOUNT_POINT_DATA_MAXSIZE];
};

struct SSceSaveDataMountResult
{
	SSceSaveDataMountPoint sceSaveDataMountPoint;
	uint32_t requiredBlocks;
	uint64_t unused;
	uint64_t mountStatus;
	uint8_t reserved[28];
	int32_t align1;
};

int PSET_SYSV_ABI Pset_sceSaveDataMount2(SSceSaveDataMount2* pMount, SSceSaveDataMountResult* pMountResult);


int PSET_SYSV_ABI Pset_sceSaveDataAbort(void);
int PSET_SYSV_ABI Pset_sceSaveDataBackup(void);
int PSET_SYSV_ABI Pset_sceSaveDataBindPsnAccount(void);
int PSET_SYSV_ABI Pset_sceSaveDataBindPsnAccountForSystemBackup(void);
int PSET_SYSV_ABI Pset_sceSaveDataChangeDatabase(void);
int PSET_SYSV_ABI Pset_sceSaveDataChangeInternal(void);
int PSET_SYSV_ABI Pset_sceSaveDataCheckBackupData(void);
int PSET_SYSV_ABI Pset_sceSaveDataCheckBackupDataInternal(void);
int PSET_SYSV_ABI Pset_sceSaveDataCheckCloudData(void);
int PSET_SYSV_ABI Pset_sceSaveDataCheckIpmiIfSize(void);
int PSET_SYSV_ABI Pset_sceSaveDataCheckSaveDataBroken(void);
int PSET_SYSV_ABI Pset_sceSaveDataCheckSaveDataVersion(void);
int PSET_SYSV_ABI Pset_sceSaveDataCheckSaveDataVersionLatest(void);
int PSET_SYSV_ABI Pset_sceSaveDataClearProgress(void);
int PSET_SYSV_ABI Pset_sceSaveDataCopy5(void);
int PSET_SYSV_ABI Pset_sceSaveDataCreateUploadData(void);
int PSET_SYSV_ABI Pset_sceSaveDataDebug(void);
int PSET_SYSV_ABI Pset_sceSaveDataDebugCleanMount(void);
int PSET_SYSV_ABI Pset_sceSaveDataDebugCompiledSdkVersion(void);
int PSET_SYSV_ABI Pset_sceSaveDataDebugCreateSaveDataRoot(void);
int PSET_SYSV_ABI Pset_sceSaveDataDebugGetThreadId(void);
int PSET_SYSV_ABI Pset_sceSaveDataDebugRemoveSaveDataRoot(void);
int PSET_SYSV_ABI Pset_sceSaveDataDebugTarget(void);
int PSET_SYSV_ABI Pset_sceSaveDataDelete(void);
int PSET_SYSV_ABI Pset_sceSaveDataDelete5(void);
int PSET_SYSV_ABI Pset_sceSaveDataDeleteAllUser(void);
int PSET_SYSV_ABI Pset_sceSaveDataDeleteCloudData(void);
int PSET_SYSV_ABI Pset_sceSaveDataDeleteUser(void);
int PSET_SYSV_ABI Pset_sceSaveDataDirNameSearch(void);
int PSET_SYSV_ABI Pset_sceSaveDataDirNameSearchInternal(void);
int PSET_SYSV_ABI Pset_sceSaveDataDownload(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetAllSize(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetAppLaunchedUser(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetAutoUploadRequestInfo(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetAutoUploadSetting(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetBoundPsnAccountCount(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetClientThreadPriority(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetCloudQuotaInfo(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetDataBaseFilePath(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetEventInfo(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetEventResult(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetFormat(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetMountedSaveDataCount(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetMountInfo(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetParam(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetProgress(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetSaveDataCount(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetSaveDataMemory(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetSaveDataMemory2(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetSaveDataRootDir(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetSaveDataRootPath(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetSaveDataRootUsbPath(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetSavePoint(void);
int PSET_SYSV_ABI Pset_sceSaveDataGetUpdatedDataCount(void);
int PSET_SYSV_ABI Pset_sceSaveDataInitialize(void);
int PSET_SYSV_ABI Pset_sceSaveDataInitialize2(void);
int PSET_SYSV_ABI Pset_sceSaveDataInitialize3(void);
int PSET_SYSV_ABI Pset_sceSaveDataInitializeForCdlg(void);
int PSET_SYSV_ABI Pset_sceSaveDataIsDeletingUsbDb(void);
int PSET_SYSV_ABI Pset_sceSaveDataIsMounted(void);
int PSET_SYSV_ABI Pset_sceSaveDataLoadIcon(void);
int PSET_SYSV_ABI Pset_sceSaveDataMount(void);
int PSET_SYSV_ABI Pset_sceSaveDataMount5(void);
int PSET_SYSV_ABI Pset_sceSaveDataMountInternal(void);
int PSET_SYSV_ABI Pset_sceSaveDataMountSys(void);
int PSET_SYSV_ABI Pset_sceSaveDataPromote5(void);
int PSET_SYSV_ABI Pset_sceSaveDataRebuildDatabase(void);
int PSET_SYSV_ABI Pset_sceSaveDataRegisterEventCallback(void);
int PSET_SYSV_ABI Pset_sceSaveDataRestoreBackupData(void);
int PSET_SYSV_ABI Pset_sceSaveDataRestoreLoadSaveDataMemory(void);
int PSET_SYSV_ABI Pset_sceSaveDataSaveIcon(void);
int PSET_SYSV_ABI Pset_sceSaveDataSetAutoUploadSetting(void);
int PSET_SYSV_ABI Pset_sceSaveDataSetEventInfo(void);
int PSET_SYSV_ABI Pset_sceSaveDataSetParam(void);
int PSET_SYSV_ABI Pset_sceSaveDataSetSaveDataLibraryUser(void);
int PSET_SYSV_ABI Pset_sceSaveDataSetSaveDataMemory(void);
int PSET_SYSV_ABI Pset_sceSaveDataSetSaveDataMemory2(void);
int PSET_SYSV_ABI Pset_sceSaveDataSetupSaveDataMemory(void);
int PSET_SYSV_ABI Pset_sceSaveDataSetupSaveDataMemory2(void);
int PSET_SYSV_ABI Pset_sceSaveDataShutdownStart(void);
int PSET_SYSV_ABI Pset_sceSaveDataSupportedFakeBrokenStatus(void);
int PSET_SYSV_ABI Pset_sceSaveDataSyncCloudList(void);
int PSET_SYSV_ABI Pset_sceSaveDataSyncSaveDataMemory(void);
int PSET_SYSV_ABI Pset_sceSaveDataTerminate(void);
int PSET_SYSV_ABI Pset_sceSaveDataTransferringMount(void);
int PSET_SYSV_ABI Pset_sceSaveDataUmount(void);
int PSET_SYSV_ABI Pset_sceSaveDataUmountSys(void);
int PSET_SYSV_ABI Pset_sceSaveDataUmountWithBackup(void);
int PSET_SYSV_ABI Pset_sceSaveDataUnregisterEventCallback(void);
int PSET_SYSV_ABI Pset_sceSaveDataUpload(void);
int PSET_SYSV_ABI NullNameFunc02E4C4D201716422(void);
