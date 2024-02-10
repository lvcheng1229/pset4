#include <stdio.h>
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
	int retRes = pthread_create(&retTid, &attr, CPsetThread::RunThreadFunction, this);
	PSET_EXIT_AND_LOG_IF(retRes != 0, "pthread create faild");
	pthread_attr_destroy(&attr);
	pthread_join(retTid, nullptr);
}

void* CPsetThread::RunThreadFunction(void* args)
{
	CPsetThread::SysABIMainFuncttion(args);
	
}

void* PSET_SYSV_ABI CPsetThread::SysABIMainFuncttion(void* args)
{
	CPsetThread* psetThread = (CPsetThread*)args;
	((PsetMainFunction)psetThread->m_pMainFunction)(&psetThread->m_psetMainArg, CPsetThread::DefaultExitFunction);
	return nullptr;
}

void PSET_SYSV_ABI CPsetThread::DefaultExitFunction(void)
{
	PSET_LOG_INFO("default exit function");
}
