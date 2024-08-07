#pragma once
#include "overridemodule/PsetLibraryCommon.h" 

struct SSceVideoOutBufferAttribute
{
	uint32_t m_pixelFormat;
	uint32_t m_tilingMode;
	uint32_t m_aspectRatio;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_pitchInPixel;
	uint32_t m_option;
	uint32_t m_reserved0;
	uint64_t m_reserved1;
};

struct SSceVideoOutResolutionStatus
{
	uint32_t m_Width;
	uint32_t m_Height;
	uint32_t m_paneWidth;
	uint32_t m_paneHeight;
	uint64_t m_refreshRate;
	float m_screenSizeInInch;
	uint16_t m_flags;
	uint16_t _reserved0;
	uint32_t _reserved1[3];
};

struct SSceVideoOutVblankStatus
{
	uint64_t m_count; // count of vblanks after sceVideoOutOpen()
	uint64_t m_processTime; // processTime of the time of the latest vblank event
	uint64_t m_timeStampCounter;// Timestamp counter value when the latest vblank executed
	uint64_t m_reserved;
	uint8_t m_flags;// SceVideoOutVblankStatusFlags
	uint8_t m_padding[7];
};

#define SCE_VIDEO_OUT_REFRESH_RATE_59_94HZ 3
#define SCE_VIDEO_OUT_RESOLUTION_STATUS_FLAGS_OUTPUT_IN_USE  (0 << 0)

int PSET_SYSV_ABI Pset_sceVideoOutSetBufferAttribute(SSceVideoOutBufferAttribute* attri,
	uint32_t pixelFormat, uint32_t tilingMode, uint32_t aspectRatio, uint32_t width, uint32_t height,
	uint32_t pitchInPixel, uint32_t option, uint32_t reserved0, uint64_t reserved1);

int PSET_SYSV_ABI Pset_sceVideoOutOpen(int32_t userId, int32_t type, int32_t index, const void* param);
int PSET_SYSV_ABI Pset_sceVideoOutGetResolutionStatus(int32_t videoHandle, SSceVideoOutResolutionStatus* status);
int PSET_SYSV_ABI Pset_sceVideoOutGetVblankStatus(int32_t videoHandle, SSceVideoOutVblankStatus* outVBlankSatus);



int PSET_SYSV_ABI Pset_sceDbgVideoOutAddOutputModeVr60Privilege(void);
int PSET_SYSV_ABI Pset_sceDbgVideoOutAddProcessAttribute(void);
int PSET_SYSV_ABI Pset_sceDbgVideoOutRemoveProcessAttribute(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddBufferAnisoPrivilege(void);
int PSET_SYSV_ABI NullNameFunc87DE0FA368680D98(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddBuffer(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddBuffer4k2kPrivilege(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddBufferYccPrivilege(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddDisplayPositionEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddDriver(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddFlipEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddPreVblankStartEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddS3dHmdPrivilege(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddSetModeEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddStereoBuffer(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddVblankEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutAdjustColor_(void);
int PSET_SYSV_ABI Pset_sceVideoOutClose(void);
int PSET_SYSV_ABI Pset_sceVideoOutColorSettingsSetGamma_(void);
int PSET_SYSV_ABI Pset_sceVideoOutConfigureOptionsInitialize_(void);
int PSET_SYSV_ABI Pset_sceVideoOutConfigureOutputMode_(void);
int PSET_SYSV_ABI Pset_sceVideoOutConfigureOutputModeEx_(void);
int PSET_SYSV_ABI Pset_sceVideoOutConfigureOutputModeExSubmitDoneUnsafe_(void);
int PSET_SYSV_ABI Pset_sceVideoOutControlHdcpEncryption(void);
int PSET_SYSV_ABI Pset_sceVideoOutCursorDisable(void);
int PSET_SYSV_ABI Pset_sceVideoOutCursorEnable(void);
int PSET_SYSV_ABI Pset_sceVideoOutCursorIsUpdatePending(void);
int PSET_SYSV_ABI Pset_sceVideoOutCursorSet2xMagnify(void);
int PSET_SYSV_ABI Pset_sceVideoOutCursorSetHotSpot(void);
int PSET_SYSV_ABI Pset_sceVideoOutCursorSetImageAddress(void);
int PSET_SYSV_ABI Pset_sceVideoOutCursorSetPosition(void);
int PSET_SYSV_ABI Pset_sceVideoOutCursorSetPositionStereo(void);
int PSET_SYSV_ABI Pset_sceVideoOutCurveSamplePointsCalc_(void);
int PSET_SYSV_ABI Pset_sceVideoOutDeleteDisplayPositionEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutDeleteDriver(void);
int PSET_SYSV_ABI Pset_sceVideoOutDeleteFlipEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutDeletePreVblankStartEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutDeleteSetModeEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutDeleteVblankEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverAddEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverDecrementBufferLabel(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverDeleteEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverGetBufferLabelAddress(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverGetEopHandle(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverGetFinishedEopHandle(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverGetLatestEopHandle(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverIncrementBufferLabel(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverResetBufferLabel(void);
int PSET_SYSV_ABI Pset_sceVideoOutDriverTriggerEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetBufferInformation(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetBufferLabelAddress(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetCoredumpOutputMode(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetCurrentOutputMode_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetDeviceCapabilityInfo_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetDeviceInfoEx_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetDeviceInfoExOts_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetEventCount(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetEventData(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetEventId(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetFlipStatus(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetHdmiKsvList_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetHdmiMonitorInfo_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetHdmiMonitorInfoNoMask_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetPortStatusInfo_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetPortStatusInfoByBusSpecifier_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetStatusForWebcore(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetVideoOutModeByBusSpecifier_(void);
int PSET_SYSV_ABI Pset_sceVideoOutHdmiMonitorInfoIsSupportedHdcpVersion_(void);
int PSET_SYSV_ABI Pset_sceVideoOutHdmiMonitorInfoIsSupportedVideoOutMode_(void);
int PSET_SYSV_ABI Pset_sceVideoOutIsFlipPending(void);
int PSET_SYSV_ABI Pset_sceVideoOutLockBuffer(void);
int PSET_SYSV_ABI Pset_sceVideoOutModeSetAny_(void);
int PSET_SYSV_ABI Pset_sceVideoOutRazorAddCallback(void);
int PSET_SYSV_ABI Pset_sceVideoOutRazorRecallCallbacks(void);
int PSET_SYSV_ABI Pset_sceVideoOutRazorRemoveCallback(void);
int PSET_SYSV_ABI Pset_sceVideoOutRegisterBufferAttribute(void);
int PSET_SYSV_ABI Pset_sceVideoOutRegisterBuffers(void);
int PSET_SYSV_ABI Pset_sceVideoOutRegisterStereoBuffers(void);
int PSET_SYSV_ABI Pset_sceVideoOutRemoveBuffer(void);
int PSET_SYSV_ABI Pset_sceVideoOutSetBlankAfterResume(void);
int PSET_SYSV_ABI Pset_sceVideoOutSetDisplayEventPosition(void);
int PSET_SYSV_ABI Pset_sceVideoOutSetFlipRate(void);
int PSET_SYSV_ABI Pset_sceVideoOutSetGamutMetadata_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSetToneMap_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSetWindowModeMargins(void);
int PSET_SYSV_ABI Pset_sceVideoOutSubmitChangeBufferAttribute(void);
int PSET_SYSV_ABI Pset_sceVideoOutSubmitEopFlip(void);
int PSET_SYSV_ABI Pset_sceVideoOutSubmitFlip(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysAddSetModeEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysAddVblankEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysClose(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysConfigureOutputMode_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysCursorDisable(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysCursorEnable(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysCursorIsUpdatePending(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysCursorObtain(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysCursorRelease(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysCursorSetPosition(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysDeleteVblankEvent(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetColorConversionStatus(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetCurrentOutputMode_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetDeviceCapabilityInfoByBusSpecifier_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetFlipStatus(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetMonitorInfo_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetPortStatusInfo_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetPortStatusInfoByBusSpecifier_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetResolutionStatus(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysGetVblankStatus(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysIsSupportedByMonitorInfo_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysReadCrc(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysReadCrc64(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysResetAtGpuReset(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysResetZoomBuffers(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSetBlackLevel(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSetDcePipeMode(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSetDimmer(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSetDisplayParameters(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSetInvertedColors(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSetOutputCsc(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSetOverscanRatio(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSetZoomBuffers(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysSubmitSubWindowLayout(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysUpdateDisplayParameter_(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysUpdatePrivilege(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysUpdateRenderingMode(void);
int PSET_SYSV_ABI Pset_sceVideoOutSysUpdateScalerParameters(void);
int PSET_SYSV_ABI Pset_sceVideoOutUnlockBuffer(void);
int PSET_SYSV_ABI Pset_sceVideoOutUnregisterBufferAttribute(void);
int PSET_SYSV_ABI Pset_sceVideoOutUnregisterBuffers(void);
int PSET_SYSV_ABI Pset_sceVideoOutWaitVblank(void);
int PSET_SYSV_ABI NullNameFunc124ED74010241580(void);
int PSET_SYSV_ABI NullNameFunc2438D6A18553BDD9(void);
int PSET_SYSV_ABI NullNameFunc3389533D3C6878DD(void);
int PSET_SYSV_ABI NullNameFunc3F96B7D8EBA56509(void);
int PSET_SYSV_ABI NullNameFunc94C92412FE1A6876(void);
int PSET_SYSV_ABI NullNameFuncBD73297BB32EADF7(void);
int PSET_SYSV_ABI NullNameFuncC0A269161093AD7D(void);
int PSET_SYSV_ABI NullNameFuncC192387E6509325C(void);
int PSET_SYSV_ABI NullNameFuncFF9008114F7ED758(void);
int PSET_SYSV_ABI Pset_sceVideoOutAdjustColor2_(void);
int PSET_SYSV_ABI Pset_sceVideoOutGetHdmiRawEdid_(void);
int PSET_SYSV_ABI Pset_sceVideoOutAddBufferHdrPrivilege(void);
int PSET_SYSV_ABI NullNameFuncDF1AD257C5341EC8(void);
