//Generated By SceModuleGenerator
#include "pset_libSceSigninDialog.h"


int PSET_SYSV_ABI Pset_sceSigninDialogClose(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSigninDialogClose");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSigninDialogGetResult(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSigninDialogGetResult");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSigninDialogGetStatus(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSigninDialogGetStatus");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSigninDialogInitialize(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSigninDialogInitialize");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSigninDialogOpen(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSigninDialogOpen");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSigninDialogTerminate(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSigninDialogTerminate");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceSigninDialogUpdateStatus(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceSigninDialogUpdateStatus");
	return PSET_OK;
}

static const SPSET_LIB_EXPORT_SYSMBOL gSymTable_libSceSigninDialog_libSceSigninDialog[] =
{
 { 0x3373A410D1DCCA25,"Pset_sceSigninDialogClose", (void*)Pset_sceSigninDialogClose },
 { 0x9EA1BBAEA9D8C355,"Pset_sceSigninDialogGetResult", (void*)Pset_sceSigninDialogGetResult },
 { 0xDA6D3BEDA782F8F0,"Pset_sceSigninDialogGetStatus", (void*)Pset_sceSigninDialogGetStatus },
 { 0x9A56067E6A84DDF4,"Pset_sceSigninDialogInitialize", (void*)Pset_sceSigninDialogInitialize },
 { 0x265A49568456BFB5,"Pset_sceSigninDialogOpen", (void*)Pset_sceSigninDialogOpen },
 { 0x2D79664BA3EF25D5,"Pset_sceSigninDialogTerminate", (void*)Pset_sceSigninDialogTerminate },
 { 0x070DF59624C54F70,"Pset_sceSigninDialogUpdateStatus", (void*)Pset_sceSigninDialogUpdateStatus },
 SPSET_LIB_EXPORT_FUNTCTION_END
};

static const SPSET_EXPORT_LIB gLibTable_libSceSigninDialog[] =
{
 {"libSceSigninDialog", gSymTable_libSceSigninDialog_libSceSigninDialog },
 SPSET_EXPORT_LIB_END
};

static const SPSET_EXPORT_MODULE gExportModule_libSceSigninDialog("libSceSigninDialog", gLibTable_libSceSigninDialog);