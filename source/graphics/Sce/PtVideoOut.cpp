#include <vector>
#include "graphics\RHI\RHIDeviceCommand.h"
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

	TestRHIFunc(m_glfwWindow);
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


