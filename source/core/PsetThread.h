#pragma once
#include <include/pthread.h>
#include "Common.h"

typedef void (PS4API* PsetExitFunction)();
typedef void* (PS4API* PsetMainFunction)(void*, PsetExitFunction pExit);

class CPsetThread
{
public:
	CPsetThread(void* minFunc);
	void Run();
	static void* RunThreadFunction(void* args);
private:
	static void PS4API DefaultExitFunction(void);

	void* m_pMainFunction;
};