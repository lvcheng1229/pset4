#pragma once
#include "overridemodule\PsetLibraryCommon.h" 

struct  SSceKernelEqueue
{
	uint32_t valid;
	uint32_t fRefs;
	uint32_t wait;
	HANDLE hIOCP;
};


struct SceKernelEvent
{
	uintptr_t	ident;		/* identifier for this event */
	short		filter;		/* filter for event */
	uint16_t	flags;
	uint32_t	fflags;
	intptr_t	data;
	void* udata;		/* opaque user data identifier */
};


int PSET_SYSV_ABI Pset_sceKernelWaitEqueue(SSceKernelEqueue* sceKernalEqueue, SceKernelEvent* sceKernelEvent, int num, int* out, uint32_t* sceKernelUseconds);