#include "TestOverrideLib.h"

int PSET_SYSV_ABI sceAjmInitialize(int64_t iReserved, SceAjmContextId* const pContext)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmInitialize");
	*pContext = 0x123;
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmFinalize(const SceAjmContextId uiContext)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmFinalize");
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmModuleRegister(const SceAjmContextId uiContext, const SceAjmCodecType uiCodec, int64_t iReserved)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmModuleRegister");
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmModuleUnregister(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmModuleUnregister");
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmBatchJobControlBufferRa(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmBatchJobControlBufferRa");
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmBatchJobRunSplitBufferRa(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmBatchJobRunSplitBufferRa");
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmBatchStartBuffer(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmBatchJobRunSplitBufferRa");
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmBatchWait(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmBatchJobRunSplitBufferRa");
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmInstanceCreate(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmBatchJobRunSplitBufferRa");
	return PSET_OK;
}


int PSET_SYSV_ABI sceAjmInstanceDestroy(void)
{
	PSET_LOG_UNIMPLEMENTED("unimplemented function: sceAjmBatchJobRunSplitBufferRa");
	return PSET_OK;
}

static const SPSET_LIB_EXPORT_FUNTCTION gFunctionTableSceAjmlibSceAjm[] =
{
	{ 0x7660F26CDFFF167F, (void*)sceAjmBatchJobControlBufferRa },
	{ 0xEE37405CAFB67CCA, (void*)sceAjmBatchJobRunSplitBufferRa },
	{ 0x7C5164934C5F196B, (void*)sceAjmBatchStartBuffer },
	{ 0xFEA2EC7C3032C086, (void*)sceAjmBatchWait },
	{ 0x307BABEAA0AC52EB, (void*)sceAjmFinalize },
	{ 0x765FB87874B352EE, (void*)sceAjmInitialize },
	{ 0x031A03AC8369E09F, (void*)sceAjmInstanceCreate },
	{ 0x45B2DBB8ABFCCE1A, (void*)sceAjmInstanceDestroy },
	{ 0x43777216EC069FAE, (void*)sceAjmModuleRegister },
	{ 0x5A2EC3B652D5F8A2, (void*)sceAjmModuleUnregister },
	SPSET_LIB_EXPORT_FUNTCTION_END
};

static const SPSET_EXPORT_LIB gLibTableSceAjm[] =
{
	{ "libSceAjm", gFunctionTableSceAjmlibSceAjm },
	SPSET_EXPORT_LIB_END
};

static const SPSET_EXPORT_MODULE gExportModuleSceAjm("libSceAjm.prx", gLibTableSceAjm);
