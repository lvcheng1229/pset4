#pragma once
#include <include/pthread.h>
#include "Common.h"

typedef void (PSET_SYSV_ABI* PsetExitFunction)();
typedef void* (PSET_SYSV_ABI* PsetMainFunction)(void*, PsetExitFunction pExit);

class CPsetThread
{
public:
	CPsetThread(void* minFunc);
	void Run();
	static void* RunThreadFunction(void* args);
private:
	static void PSET_SYSV_ABI DefaultExitFunction(void);

	void* m_pMainFunction;
};