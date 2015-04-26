#include "VirtualMachine.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "VirtualMachine.h"
#include "Machine.h"
#include "Util.h"

extern TVMMainEntry VMLoadModule(const char *);

TVMStatus VMStart(int tickms, int machinetickms, int argc, char *argv[])
{

	int success = 0;

 /*  Order recommended by the professor
	*  1. VMLoadModule
	*  2. MachineInitialize
	*  3. MachineRequestAlarm
	*  4. MachineEnableSignals
	*  5. VMMain (whatever VMLoadModule returned)
	*/

	TVMMainEntry e = VMLoadModule(argv[0]);
	MachineInitialize(machinetickms * 1000);

	//MachineContextCreate

	
	if(e != NULL) {
		
		MachineEnableSignals();
		MachineRequestAlarm(tickms * 1000, alarmCallback, NULL);
		e(argc, argv);
		success = 1;

	} 
		
	//VMUnloadModule();
	//MachineTerminate();

	if(success)
		return VM_STATUS_SUCCESS;
	else
		return VM_STATUS_FAILURE;

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
	// Set the clock
	clock = tick;
	
	// Check to make sure timout is not infinite
	if(tick == VM_TIMEOUT_INFINITE) 
		return VM_STATUS_ERROR_INVALID_PARAMETER;

	TMachineSignalState OldState;
	MachineSuspendSignals(&OldState);
	while(clock > 0) {
		// wait
	}
	MachineResumeSignals(&OldState);

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

