//Generated By SceModuleGenerator
#include "pset_libSceNpCommerce.h"


int PSET_SYSV_ABI Pset_sceNpCommerceDialogClose(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceDialogClose");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceDialogGetResult(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceDialogGetResult");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceDialogGetStatus(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceDialogGetStatus");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceDialogInitialize(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceDialogInitialize");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceDialogInitializeInternal(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceDialogInitializeInternal");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceDialogOpen(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceDialogOpen");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceDialogTerminate(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceDialogTerminate");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceDialogUpdateStatus(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceDialogUpdateStatus");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceHidePsStoreIcon(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceHidePsStoreIcon");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceSetPsStoreIconLayout(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceSetPsStoreIconLayout");
	return PSET_OK;
}

int PSET_SYSV_ABI Pset_sceNpCommerceShowPsStoreIcon(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: Pset_sceNpCommerceShowPsStoreIcon");
	return PSET_OK;
}

static const SPSET_LIB_EXPORT_SYSMBOL gSymTable_libSceNpCommerce_libSceNpCommerce[] =
{
 { 0x354DDC9061CC157A, (void*)Pset_sceNpCommerceDialogClose },
 { 0xAF8D9B59C41BB596, (void*)Pset_sceNpCommerceDialogGetResult },
 { 0x0826C2FA5AAABC5D, (void*)Pset_sceNpCommerceDialogGetStatus },
 { 0xD1A4766969906A5E, (void*)Pset_sceNpCommerceDialogInitialize },
 { 0xF5988B5C0186E6B8, (void*)Pset_sceNpCommerceDialogInitializeInternal },
 { 0x0DF4820D10371236, (void*)Pset_sceNpCommerceDialogOpen },
 { 0x9BF23DD806F9D16F, (void*)Pset_sceNpCommerceDialogTerminate },
 { 0x2D1E5CC0530C0951, (void*)Pset_sceNpCommerceDialogUpdateStatus },
 { 0x76CA8256C34CD198, (void*)Pset_sceNpCommerceHidePsStoreIcon },
 { 0xB8A4C35BC864FEDB, (void*)Pset_sceNpCommerceSetPsStoreIconLayout },
 { 0x0C79B0B1AE92F137, (void*)Pset_sceNpCommerceShowPsStoreIcon },
 SPSET_LIB_EXPORT_FUNTCTION_END
};

static const SPSET_EXPORT_LIB gLibTable_libSceNpCommerce[] =
{
 {"libSceNpCommerce", gSymTable_libSceNpCommerce_libSceNpCommerce },
 SPSET_EXPORT_LIB_END
};

static const SPSET_EXPORT_MODULE gExportModule_libSceNpCommerce("libSceNpCommerce", gLibTable_libSceNpCommerce);