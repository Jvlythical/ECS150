#include "VirtualMachine.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

extern const char *mod;
extern TVMMainEntry VMLoadModule(const char *);

TVMStatus VMStart(int tickms, int machinetickms, int argc, char *argv[])
{

	void (*f)();
	
	f = VMLoadModule(mod);
	f();
	return VM_STATUS_SUCCESS;
};

TVMStatus VMThreadCreate(TVMThreadEntry entry, void *param, TVMMemorySize memsize, TVMThreadPriority prio, TVMThreadIDRef tid)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMThreadDelete(TVMThreadID thread)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMThreadActivate(TVMThreadID thread)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMThreadTerminate(TVMThreadID thread)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMThreadID(TVMThreadIDRef threadref)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMThreadState(TVMThreadID thread, TVMThreadStateRef stateref)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMThreadSleep(TVMTick tick)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMMutexCreate(TVMMutexIDRef mutexref)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMMutexDelete(TVMMutexID mutex)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMMutexQuery(TVMMutexID mutex, TVMThreadIDRef ownerref)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMMutexAcquire(TVMMutexID mutex, TVMTick timeout)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMMutexRelease(TVMMutexID mutex)
{

	return VM_STATUS_SUCCESS;
};

TVMStatus VMFileOpen(const char *filename, int flags, int mode, int *filedescriptor)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMFileClose(int filedescriptor)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMFileRead(int filedescriptor, void *data, int *length)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMFileWrite(int filedescriptor, void *data, int *length)
{
	
	return VM_STATUS_SUCCESS;
};

TVMStatus VMFileSeek(int filedescriptor, int offset, int whence, int *newoffset)
{
	
	return VM_STATUS_SUCCESS;
};

