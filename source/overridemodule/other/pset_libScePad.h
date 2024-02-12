#pragma once
#include "overridemodule/PsetLibraryCommon.h" 

#define SCE_PAD_DEVICE_CLASS_INVALID -1;
#define SCE_PAD_DEVICE_CLASS_STANDARD  0;
#define SCE_PAD_DEVICE_CLASS_GUITAR  1;
#define SCE_PAD_DEVICE_CLASS_DRUM  2;
#define SCE_PAD_DEVICE_CLASS_DJ_TURNTABLE  3;
#define SCE_PAD_DEVICE_CLASS_DANCEMAT  4;
#define SCE_PAD_DEVICE_CLASS_NAVIGATION  5;
#define SCE_PAD_DEVICE_CLASS_STEERING_WHEEL  6;
#define SCE_PAD_DEVICE_CLASS_STICK  7;
#define SCE_PAD_DEVICE_CLASS_FLIGHT_STICK  8;
#define SCE_PAD_DEVICE_CLASS_GUN 9;

#define SCE_PAD_CONNECTION_TYPE_LOCAL              0
#define SCE_PAD_CONNECTION_TYPE_REMOTE_VITA        1
#define SCE_PAD_CONNECTION_TYPE_REMOTE_DUALSHOCK4  2

struct SScePadTouchPadInformation
{
	float m_pixelDensity;
	struct
	{
		uint16_t x;
		uint16_t y;
	} m_resolution;
};

struct SScePadStickInformation
{
	uint8_t m_deadZoneLeft;
	uint8_t m_deadZoneRight;
};

struct SScePadControllerInformation
{
	SScePadTouchPadInformation m_touchPadInfo;
	SScePadStickInformation m_stickInfo;
	uint8_t m_connectionType;
	uint8_t m_connectedCount;
	bool m_connected;
	int64_t m_deviceClass;
	uint8_t reserve[8];
};

int PSET_SYSV_ABI Pset_scePadOpen(int32_t userID, int32_t type, int32_t index, void* param);
int PSET_SYSV_ABI Pset_scePadGetControllerInformation(int32_t handle, SScePadControllerInformation* pInfo);


int PSET_SYSV_ABI Pset_scePadClose(void);
int PSET_SYSV_ABI Pset_scePadConnectPort(void);
int PSET_SYSV_ABI Pset_scePadDeviceClassGetExtendedInformation(void);
int PSET_SYSV_ABI Pset_scePadDeviceClassParseData(void);
int PSET_SYSV_ABI Pset_scePadDeviceOpen(void);
int PSET_SYSV_ABI Pset_scePadDisableVibration(void);
int PSET_SYSV_ABI Pset_scePadDisconnectDevice(void);
int PSET_SYSV_ABI Pset_scePadDisconnectPort(void);
int PSET_SYSV_ABI Pset_scePadEnableAutoDetect(void);
int PSET_SYSV_ABI Pset_scePadEnableExtensionPort(void);
int PSET_SYSV_ABI Pset_scePadEnableSpecificDeviceClass(void);
int PSET_SYSV_ABI Pset_scePadEnableUsbConnection(void);
int PSET_SYSV_ABI Pset_scePadGetBluetoothAddress(void);
int PSET_SYSV_ABI Pset_scePadGetCapability(void);
int PSET_SYSV_ABI Pset_scePadGetDataInternal(void);
int PSET_SYSV_ABI Pset_scePadGetDeviceId(void);
int PSET_SYSV_ABI Pset_scePadGetDeviceInfo(void);
int PSET_SYSV_ABI Pset_scePadGetExtControllerInformation(void);
int PSET_SYSV_ABI Pset_scePadGetExtensionUnitInfo(void);
int PSET_SYSV_ABI Pset_scePadGetFeatureReport(void);
int PSET_SYSV_ABI Pset_scePadGetHandle(void);
int PSET_SYSV_ABI Pset_scePadGetIdleCount(void);
int PSET_SYSV_ABI Pset_scePadGetInfo(void);
int PSET_SYSV_ABI Pset_scePadGetInfoByPortType(void);
int PSET_SYSV_ABI Pset_scePadGetLicenseControllerInformation(void);
int PSET_SYSV_ABI Pset_scePadGetMotionSensorPosition(void);
int PSET_SYSV_ABI Pset_scePadGetMotionTimerUnit(void);
int PSET_SYSV_ABI Pset_scePadGetSphereRadius(void);
int PSET_SYSV_ABI Pset_scePadGetVersionInfo(void);
int PSET_SYSV_ABI Pset_scePadInit(void);
int PSET_SYSV_ABI Pset_scePadIsBlasterConnected(void);
int PSET_SYSV_ABI Pset_scePadIsDS4Connected(void);
int PSET_SYSV_ABI Pset_scePadIsLightBarBaseBrightnessControllable(void);
int PSET_SYSV_ABI Pset_scePadIsMoveConnected(void);
int PSET_SYSV_ABI Pset_scePadIsMoveReproductionModel(void);
int PSET_SYSV_ABI Pset_scePadIsValidHandle(void);
int PSET_SYSV_ABI Pset_scePadMbusInit(void);
int PSET_SYSV_ABI Pset_scePadMbusTerm(void);
int PSET_SYSV_ABI Pset_scePadOpenExt(void);
int PSET_SYSV_ABI Pset_scePadOpenExt2(void);
int PSET_SYSV_ABI Pset_scePadOutputReport(void);
int PSET_SYSV_ABI Pset_scePadRead(void);
int PSET_SYSV_ABI Pset_scePadReadBlasterForTracker(void);
int PSET_SYSV_ABI Pset_scePadReadExt(void);
int PSET_SYSV_ABI Pset_scePadReadForTracker(void);
int PSET_SYSV_ABI Pset_scePadReadHistory(void);
int PSET_SYSV_ABI Pset_scePadReadState(void);
int PSET_SYSV_ABI Pset_scePadReadStateExt(void);
int PSET_SYSV_ABI Pset_scePadResetLightBar(void);
int PSET_SYSV_ABI Pset_scePadResetLightBarAll(void);
int PSET_SYSV_ABI Pset_scePadResetLightBarAllByPortType(void);
int PSET_SYSV_ABI Pset_scePadResetOrientation(void);
int PSET_SYSV_ABI Pset_scePadResetOrientationForTracker(void);
int PSET_SYSV_ABI Pset_scePadSetAngularVelocityDeadbandState(void);
int PSET_SYSV_ABI Pset_scePadSetAutoPowerOffCount(void);
int PSET_SYSV_ABI Pset_scePadSetButtonRemappingInfo(void);
int PSET_SYSV_ABI Pset_scePadSetConnection(void);
int PSET_SYSV_ABI Pset_scePadSetExtensionReport(void);
int PSET_SYSV_ABI Pset_scePadSetFeatureReport(void);
int PSET_SYSV_ABI Pset_scePadSetForceIntercepted(void);
int PSET_SYSV_ABI Pset_scePadSetLightBar(void);
int PSET_SYSV_ABI Pset_scePadSetLightBarBaseBrightness(void);
int PSET_SYSV_ABI Pset_scePadSetLightBarBlinking(void);
int PSET_SYSV_ABI Pset_scePadSetLightBarForTracker(void);
int PSET_SYSV_ABI Pset_scePadSetLoginUserNumber(void);
int PSET_SYSV_ABI Pset_scePadSetMotionSensorState(void);
int PSET_SYSV_ABI Pset_scePadSetProcessFocus(void);
int PSET_SYSV_ABI Pset_scePadSetProcessPrivilege(void);
int PSET_SYSV_ABI Pset_scePadSetProcessPrivilegeOfButtonRemapping(void);
int PSET_SYSV_ABI Pset_scePadSetTiltCorrectionState(void);
int PSET_SYSV_ABI Pset_scePadSetUserColor(void);
int PSET_SYSV_ABI Pset_scePadSetVibration(void);
int PSET_SYSV_ABI Pset_scePadSetVibrationForce(void);
int PSET_SYSV_ABI Pset_scePadSetVrTrackingMode(void);
int PSET_SYSV_ABI Pset_scePadShareOutputData(void);
int PSET_SYSV_ABI Pset_scePadStartRecording(void);
int PSET_SYSV_ABI Pset_scePadStopRecording(void);
int PSET_SYSV_ABI Pset_scePadSwitchConnection(void);
int PSET_SYSV_ABI Pset_scePadVertualDeviceAddDevice(void);
int PSET_SYSV_ABI Pset_scePadVirtualDeviceAddDevice(void);
int PSET_SYSV_ABI Pset_scePadVirtualDeviceDeleteDevice(void);
int PSET_SYSV_ABI Pset_scePadVirtualDeviceGetRemoteSetting(void);
int PSET_SYSV_ABI Pset_scePadVirtualDeviceInsertData(void);
int PSET_SYSV_ABI NullNameFunc51E514BCD3A05CA5(void);
int PSET_SYSV_ABI NullNameFunc89C9237E393DA243(void);
int PSET_SYSV_ABI NullNameFuncEF103E845B6F0420(void);
