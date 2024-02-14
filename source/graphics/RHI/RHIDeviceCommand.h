#pragma once

class CRHIDeviceCommand
{
public:
	virtual void Init(void* windowHandle) = 0;
	virtual void TestMainLoop() = 0;
private:
};

CRHIDeviceCommand* GetRHIDevieCommnad();
