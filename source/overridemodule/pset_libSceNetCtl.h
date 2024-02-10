#pragma once
#include "PsetLibraryCommon.h" 
int PSET_SYSV_ABI Pset_sceNetBweCheckCallbackIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetBweClearEventIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetBweFinishInternetConnectionTestIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetBweGetInfoIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetBweRegisterCallbackIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetBweSetInternetConnectionTestResultIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetBweStartInternetConnectionTestBandwidthTestIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetBweStartInternetConnectionTestIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetBweUnregisterCallbackIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetInfoV6(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetResultV6(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetStateV6(void);
int PSET_SYSV_ABI Pset_sceNetCtlRegisterCallbackV6(void);
int PSET_SYSV_ABI Pset_sceNetCtlUnregisterCallbackV6(void);
int PSET_SYSV_ABI Pset_sceNetCtlCheckCallback(void);
int PSET_SYSV_ABI Pset_sceNetCtlCheckCallbackForLibIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlClearEventForLibIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlClearEventIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlConnectConfIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlConnectIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlConnectWithRetryIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlDisableBandwidthManagementIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlDisconnectIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlEnableBandwidthManagementIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetBandwidthInfoIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetEtherLinkMode(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetIfStat(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetInfo(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetInfoIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetNatInfo(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetNatInfoIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetNetEvConfigInfoIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetResult(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetResultIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetScanInfoBssidForSsidListScanIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetScanInfoBssidIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetScanInfoByBssidIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetScanInfoForSsidListScanIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetScanInfoForSsidScanIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetState(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetState2IpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetStateIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlGetWifiType(void);
int PSET_SYSV_ABI Pset_sceNetCtlInit(void);
int PSET_SYSV_ABI Pset_sceNetCtlIsBandwidthManagementEnabledIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlRegisterCallback(void);
int PSET_SYSV_ABI Pset_sceNetCtlRegisterCallbackForLibIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlRegisterCallbackIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlScanIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlSetErrorNotificationEnabledIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlSetStunWithPaddingFlagIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlTerm(void);
int PSET_SYSV_ABI Pset_sceNetCtlUnregisterCallback(void);
int PSET_SYSV_ABI Pset_sceNetCtlUnregisterCallbackForLibIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlUnregisterCallbackIpcInt(void);
int PSET_SYSV_ABI Pset_sceNetCtlUnsetStunWithPaddingFlagIpcInt(void);
int PSET_SYSV_ABI NullNameFuncD8DCB6973537A3DC(void);
int PSET_SYSV_ABI Pset_sceNetCtlCheckCallbackForNpToolkit(void);
int PSET_SYSV_ABI Pset_sceNetCtlClearEventForNpToolkit(void);
int PSET_SYSV_ABI Pset_sceNetCtlRegisterCallbackForNpToolkit(void);
int PSET_SYSV_ABI Pset_sceNetCtlUnregisterCallbackForNpToolkit(void);
int PSET_SYSV_ABI Pset_sceNetCtlApCheckCallback(void);
int PSET_SYSV_ABI Pset_sceNetCtlApClearEvent(void);
int PSET_SYSV_ABI Pset_sceNetCtlApGetConnectInfo(void);
int PSET_SYSV_ABI Pset_sceNetCtlApGetInfo(void);
int PSET_SYSV_ABI Pset_sceNetCtlApGetResult(void);
int PSET_SYSV_ABI Pset_sceNetCtlApGetState(void);
int PSET_SYSV_ABI Pset_sceNetCtlApInit(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRegisterCallback(void);
int PSET_SYSV_ABI Pset_sceNetCtlApStop(void);
int PSET_SYSV_ABI Pset_sceNetCtlApTerm(void);
int PSET_SYSV_ABI Pset_sceNetCtlApUnregisterCallback(void);
int PSET_SYSV_ABI Pset_sceNetCtlApAppInitWpaKey(void);
int PSET_SYSV_ABI Pset_sceNetCtlApAppInitWpaKeyForQa(void);
int PSET_SYSV_ABI Pset_sceNetCtlApAppStartWithRetry(void);
int PSET_SYSV_ABI Pset_sceNetCtlApAppStartWithRetryPid(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRestart(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpCheckCallback(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpClearEvent(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpGetInfo(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpGetResult(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpGetState(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpRegisterCallback(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpStart(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpStartConf(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpStartWithRetry(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpStop(void);
int PSET_SYSV_ABI Pset_sceNetCtlApRpUnregisterCallback(void);