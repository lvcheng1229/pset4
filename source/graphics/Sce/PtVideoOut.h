#include <stdint.h>
#include "core\PtUtil.h"

struct GLFWwindow;
class CPtDisplay
{
public:
	CPtDisplay();
	inline Vec2ui GetDisplaySize() { return m_ptDisplayWidthHeight; }

private:
	const Vec2ui m_ptDisplayWidthHeight = { 1280 ,720 };
	GLFWwindow* m_glfwWindow;
};

struct SVblankStatus
{
	uint64_t m_processTime;
	uint64_t m_timeStampCounter;
	uint64_t m_timeStampStartCounter;
};

class CPtVideoOut
{
public:
	CPtVideoOut();
	inline SVblankStatus& GetBlankStatus() { return m_vblankSatus; };
	inline Vec2ui GetDisplaySize() { return m_display.GetDisplaySize(); }
private:
	CPtDisplay m_display;
	SVblankStatus m_vblankSatus;
};

int AddVideoOut();
CPtVideoOut* GetVideoOut(int videoIndex);