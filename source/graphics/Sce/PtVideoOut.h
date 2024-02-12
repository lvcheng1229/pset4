#include <stdint.h>
#include "core\PtUtil.h"

class GLFWwindow;
class CPtDisplay
{
public:
	CPtDisplay();
	inline Vec2ui GetDisplaySize() { return m_ptDisplayWidthHeight; }

private:
	const Vec2ui m_ptDisplayWidthHeight = { 1920 ,1080 };
	GLFWwindow* m_glfwWindow;
};

class CPtVideoOut
{
public:
	inline Vec2ui GetDisplaySize() { return m_display.GetDisplaySize(); }
private:
	CPtDisplay m_display;
};

int AddVideoOut();
CPtVideoOut* GetVideoOut(int videoIndex);