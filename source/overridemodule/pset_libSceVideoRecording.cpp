//Generated By SceModuleGenerator
#include "pset_libSceVideoRecording.h"


int PSET_SYSV_ABI Pset_sceVideoRecordingClose(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingClose");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingCopyBGRAtoNV12(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingCopyBGRAtoNV12");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingGetInfo(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingGetInfo");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingGetStatus(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingGetStatus");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingInit(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingInit");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingOpen(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingOpen");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingOpen2(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingOpen2");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingQueryHddSize2(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingQueryHddSize2");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingQueryMemSize(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingQueryMemSize");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingQueryMemSize2(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingQueryMemSize2");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingSetInfo(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingSetInfo");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingStart(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingStart");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingStop(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingStop");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceVideoRecordingTerm(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceVideoRecordingTerm");
	return PSET_OK;
}

static const SPSET_LIB_EXPORT_SYSMBOL gSymTable_libSceVideoRecording_libSceVideoRecording[] =
{
 { 0x287BE43D024330B9,"Pset_sceVideoRecordingClose", (void*)Pset_sceVideoRecordingClose },
 { 0x40D83C2868B1AF38,"Pset_sceVideoRecordingCopyBGRAtoNV12", (void*)Pset_sceVideoRecordingCopyBGRAtoNV12 },
 { 0xB00EBEE5775BA8C0,"Pset_sceVideoRecordingGetInfo", (void*)Pset_sceVideoRecordingGetInfo },
 { 0x7D9250CC52B81AFE,"Pset_sceVideoRecordingGetStatus", (void*)Pset_sceVideoRecordingGetStatus },
 { 0x4575C197AFE28A6A,"Pset_sceVideoRecordingInit", (void*)Pset_sceVideoRecordingInit },
 { 0xED0A1363356ECEDA,"Pset_sceVideoRecordingOpen", (void*)Pset_sceVideoRecordingOpen },
 { 0xB36F1D6A5070A768,"Pset_sceVideoRecordingOpen2", (void*)Pset_sceVideoRecordingOpen2 },
 { 0xA8E9A1E87BC7852B,"Pset_sceVideoRecordingQueryHddSize2", (void*)Pset_sceVideoRecordingQueryHddSize2 },
 { 0x66F5B34B6C1320C7,"Pset_sceVideoRecordingQueryMemSize", (void*)Pset_sceVideoRecordingQueryMemSize },
 { 0x6C662463AAB76C8C,"Pset_sceVideoRecordingQueryMemSize2", (void*)Pset_sceVideoRecordingQueryMemSize2 },
 { 0x15CF2AC652883584,"Pset_sceVideoRecordingSetInfo", (void*)Pset_sceVideoRecordingSetInfo },
 { 0xB56A1EF48946021B,"Pset_sceVideoRecordingStart", (void*)Pset_sceVideoRecordingStart },
 { 0x38E171ACC63E99F2,"Pset_sceVideoRecordingStop", (void*)Pset_sceVideoRecordingStop },
 { 0x81666292847E83D8,"Pset_sceVideoRecordingTerm", (void*)Pset_sceVideoRecordingTerm },
 SPSET_LIB_EXPORT_FUNTCTION_END
};

static const SPSET_EXPORT_LIB gLibTable_libSceVideoRecording[] =
{
 {"libSceVideoRecording", gSymTable_libSceVideoRecording_libSceVideoRecording },
 SPSET_EXPORT_LIB_END
};

static const SPSET_EXPORT_MODULE gExportModule_libSceVideoRecording("libSceVideoRecording", gLibTable_libSceVideoRecording);