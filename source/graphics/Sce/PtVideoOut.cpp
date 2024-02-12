#include <vector>

#define GLFW_INCLUDE_VULKAN
#include"GLFW\glfw3.h"

#include "PtVideoOut.h"
static CPtVideoOut* gVideoOuts[3];
static uint32_t gVideoOutNum = 0;

CPtDisplay::CPtDisplay()
{
	//glfwInit();
	//
	//// for vulkan
	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//// a hardware display shouldn't be resizable of course.
	//// well, a virtual one should be:) ,but currently we just disable resize.
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//
	//m_window = glfwCreateWindow(VirtualDisplayWidth, VirtualDisplayHeight, GPCS4_APP_NAME, nullptr, nullptr);
	//glfwSetWindowUserPointer(m_window, this);
	//
	//glfwSetWindowSizeCallback(m_window, windowResizeCallback);
	//glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
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


