#include <vector>
#include <Windows.h>

#include "graphics\Vulkan\PtVkDeviceCommand.h"
#include "graphics\RHI\DynamicRHI.h"

#define GLFW_INCLUDE_VULKAN
#include"GLFW\glfw3.h"

#include "PtVideoOut.h"
static CPtVideoOut* gVideoOuts[3];
static uint32_t gVideoOutNum = 0;

static void GLFWWindowSizeFun(GLFWwindow* window, int width, int height)
{
}

static void GLFWFramebufferSizeFun(GLFWwindow* window, int width, int height)
{
}

CPtDisplay::CPtDisplay()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	m_glfwWindow = glfwCreateWindow(m_ptDisplayWidthHeight.x, m_ptDisplayWidthHeight.y, "pset4", nullptr, nullptr);
	glfwSetWindowUserPointer(m_glfwWindow, this);
	
	glfwSetWindowSizeCallback(m_glfwWindow, GLFWWindowSizeFun);
	glfwSetFramebufferSizeCallback(m_glfwWindow, GLFWFramebufferSizeFun);

	InitDynamicRHI(m_glfwWindow);
}

CPtVideoOut::CPtVideoOut()
{
	HANDLE     processHandle = GetCurrentProcess();
	FILETIME createTime, exitTime, kernelTime, userTime;
	GetProcessTimes(processHandle, &createTime, &exitTime, &kernelTime, &userTime); //ms

	m_vblankSatus.m_processTime = *(uint64_t*)(&userTime);

	uint64_t lpPerformanceCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&lpPerformanceCount);
	m_vblankSatus.m_timeStampCounter = lpPerformanceCount;
	m_vblankSatus.m_timeStampStartCounter = lpPerformanceCount;
}

int AddVideoOut()
{
	uint32_t currentVideoOutIndex = gVideoOutNum;
	gVideoOutNum++;

	gVideoOuts[currentVideoOutIndex] = new CPtVideoOut();
	return currentVideoOutIndex;
}

CPtVideoOut* GetVideoOut(int videoIndex)
{
	return gVideoOuts[videoIndex];
}


