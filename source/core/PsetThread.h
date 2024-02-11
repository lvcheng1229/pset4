#pragma once
#include <memory>
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
	static void* PSET_SYSV_ABI SysABIMainFuncttion(void* args);
	static void PSET_SYSV_ABI DefaultExitFunction(void);
private:

	void* m_pMainFunction;
	struct SPsetMainArgs
	{
		uint64_t    m_argc = 1;
		const char* m_argv[1] = { "eboot.bin" };
	};
	SPsetMainArgs m_psetMainArg;
};