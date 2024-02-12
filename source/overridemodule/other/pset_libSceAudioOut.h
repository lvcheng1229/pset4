#pragma once
#include "overridemodule/PsetLibraryCommon.h" 
struct SAudioOutContext;
class CAudioOut
{
public:
	int Init(int32_t userID, int32_t type, int32_t index, int32_t length, uint32_t freq, uint32_t param);
	void Reset();
	int32_t AudioOutput(const void* ptr);
private:
	std::unique_ptr<SAudioOutContext> m_audioOutContext;
};

struct SSceAudioOutOutputParam
{
	int32_t handle;
	int32_t align;
	void* ptr;
};

int PSET_SYSV_ABI Pset_sceAudioOutOpen(int32_t userID, int32_t type, int32_t index, int32_t length, uint32_t freq, uint32_t param);
int PSET_SYSV_ABI Pset_sceAudioOutOutput(int32_t handle, const void* ptr);
int PSET_SYSV_ABI Pset_sceAudioOutOutputs(SSceAudioOutOutputParam* params, uint64_t count);


int PSET_SYSV_ABI Pset_sceAudioOutDeviceIdOpen(void);
int PSET_SYSV_ABI Pset_sceAudioDeviceControlGet(void);
int PSET_SYSV_ABI Pset_sceAudioDeviceControlSet(void);
int PSET_SYSV_ABI Pset_sceAudioOutA3dControl(void);
int PSET_SYSV_ABI Pset_sceAudioOutA3dExit(void);
int PSET_SYSV_ABI Pset_sceAudioOutA3dInit(void);
int PSET_SYSV_ABI Pset_sceAudioOutAttachToApplicationByPid(void);
int PSET_SYSV_ABI Pset_sceAudioOutChangeAppModuleState(void);
int PSET_SYSV_ABI Pset_sceAudioOutClose(void);
int PSET_SYSV_ABI Pset_sceAudioOutDetachFromApplicationByPid(void);
int PSET_SYSV_ABI Pset_sceAudioOutExConfigureOutputMode(void);
int PSET_SYSV_ABI Pset_sceAudioOutExGetSystemInfo(void);
int PSET_SYSV_ABI Pset_sceAudioOutExPtClose(void);
int PSET_SYSV_ABI Pset_sceAudioOutExPtGetLastOutputTime(void);
int PSET_SYSV_ABI Pset_sceAudioOutExPtOpen(void);
int PSET_SYSV_ABI Pset_sceAudioOutExSystemInfoIsSupportedAudioOutExMode(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetFocusEnablePid(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetHandleStatusInfo(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetInfo(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetInfoOpenNum(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetLastOutputTime(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetPortState(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetSimulatedBusUsableStatusByBusType(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetSimulatedHandleStatusInfo(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetSimulatedHandleStatusInfo2(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetSparkVss(void);
int PSET_SYSV_ABI Pset_sceAudioOutGetSystemState(void);
int PSET_SYSV_ABI Pset_sceAudioOutInit(void);
int PSET_SYSV_ABI Pset_sceAudioOutInitIpmiGetSession(void);
int PSET_SYSV_ABI Pset_sceAudioOutMasteringGetState(void);
int PSET_SYSV_ABI Pset_sceAudioOutMasteringInit(void);
int PSET_SYSV_ABI Pset_sceAudioOutMasteringSetParam(void);
int PSET_SYSV_ABI Pset_sceAudioOutMasteringTerm(void);
int PSET_SYSV_ABI Pset_sceAudioOutMbusInit(void);
int PSET_SYSV_ABI Pset_sceAudioOutOpenEx(void);
int PSET_SYSV_ABI Pset_sceAudioOutPtClose(void);
int PSET_SYSV_ABI Pset_sceAudioOutPtGetLastOutputTime(void);
int PSET_SYSV_ABI Pset_sceAudioOutPtOpen(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetConnections(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetConnectionsForUser(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetDevConnection(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetHeadphoneOutMode(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetJediJackVolume(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetJediSpkVolume(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetMainOutput(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetMixLevelPadSpk(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetMorpheusParam(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetMorpheusWorkingMode(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetPortConnections(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetPortStatuses(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetRecMode(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetSparkParam(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetUsbVolume(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetVolume(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetVolumeDown(void);
int PSET_SYSV_ABI Pset_sceAudioOutStartAuxBroadcast(void);
int PSET_SYSV_ABI Pset_sceAudioOutStartSharePlay(void);
int PSET_SYSV_ABI Pset_sceAudioOutStopAuxBroadcast(void);
int PSET_SYSV_ABI Pset_sceAudioOutStopSharePlay(void);
int PSET_SYSV_ABI Pset_sceAudioOutSuspendResume(void);
int PSET_SYSV_ABI Pset_sceAudioOutSysConfigureOutputMode(void);
int PSET_SYSV_ABI Pset_sceAudioOutSysGetHdmiMonitorInfo(void);
int PSET_SYSV_ABI Pset_sceAudioOutSysGetSystemInfo(void);
int PSET_SYSV_ABI Pset_sceAudioOutSysHdmiMonitorInfoIsSupportedAudioOutMode(void);
int PSET_SYSV_ABI Pset_sceAudioOutSystemControlGet(void);
int PSET_SYSV_ABI Pset_sceAudioOutSystemControlSet(void);
int PSET_SYSV_ABI Pset_sceAudioOutSparkControlSetEqCoef(void);
int PSET_SYSV_ABI Pset_sceAudioOutSetSystemDebugState(void);
