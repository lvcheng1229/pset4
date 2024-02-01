#include "core/Log.h"
#include "PsetThread.h"


static constexpr size_t defaultStackSize = 1024 * 1024 * 256; // 256 M

CPsetThread::CPsetThread(void* minFunc)
	:m_pMainFunction(minFunc)
{

}

void CPsetThread::Run()
{
	pthread_t retTid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, defaultStackSize); //TODO: get stack size from program parameters
	bool retRes = pthread_create(&retTid, &attr, CPsetThread::RunThreadFunction, this);
	PSET_EXIT_AND_LOG_IF(retRes != 0, "pthread create faild");
	pthread_attr_destroy(&attr);
}

void* CPsetThread::RunThreadFunction(void* args)
{
	struct SPsetMainArgs
	{
		uint64_t    m_argc = 1;
		const char* m_argv[1] = { "eboot.bin" };
	};

	SPsetMainArgs psetMainArg;
	CPsetThread* psetThread = (CPsetThread*)args;
	((PsetMainFunction)psetThread->m_pMainFunction)(&psetMainArg, CPsetThread::DefaultExitFunction);
	return nullptr;
}

void PS4API CPsetThread::DefaultExitFunction(void)
{
	PSET_LOG_INFO("default exit function");
}
