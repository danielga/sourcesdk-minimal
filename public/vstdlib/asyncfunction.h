
#include "tier1/utldelegate.h"

#ifndef ASYNCFUNCTION_H
#define ASYNCFUNCTION_H
#pragma once

#if !defined( STATIC_LINK )
#ifdef VSTDLIB_DLL_EXPORT
#define JOB_INTERFACE	DLL_EXPORT
#define JOB_OVERLOAD	DLL_GLOBAL_EXPORT
#define JOB_CLASS		DLL_CLASS_EXPORT
#else
#define JOB_INTERFACE	DLL_IMPORT
#define JOB_OVERLOAD	DLL_GLOBAL_IMPORT
#define JOB_CLASS		DLL_CLASS_IMPORT
#endif
#else
#define JOB_INTERFACE	extern
#define JOB_OVERLOAD
#define JOB_CLASS
#endif

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

MAKE_NONCASTABLE_INTEGER_TYPE( AsyncFunctionHandle_t, int32, -1 );
#define INVALID_ASYNC_FUNCTION_HANDLE AsyncFunctionHandle_t(-1)

// interface to schedule functions on an async thread
// The function returns a float which is the time in seconds for the next call, -1 to stop calling this function until it is rescheduled
class IAsyncScheduledFunctions
{
public:
	virtual ~IAsyncScheduledFunctions() {}
	// NOTE: will auto-init at first register
	virtual void Init() = 0;

	// shuts down the thread and stops all calls.  Synchronous - at return the thread is stopped
	virtual void Shutdown() = 0;

	// register a function to get called async to the main loop at regular intervals.
	// function returns the interval for the next call
	// returns a handle that can be used to reschedule
	virtual AsyncFunctionHandle_t RegisterScheduledFunction( const CUtlDelegate<float()> &fn, double flTimeToCall ) = 0;
	
	// change the next scheduled time to call the function (e.g. new data arrived, force a call now)
	virtual bool RescheduleFunction( AsyncFunctionHandle_t nHandle, double flTimeToCall ) = 0;
	
	// free this handle - it may get re-used by a subsequent call to RegisterScheduledFunction
	virtual void UnregisterScheduledFunction( AsyncFunctionHandle_t nHandle ) = 0;
};

JOB_INTERFACE IAsyncScheduledFunctions *g_pScheduledFunctions;

#endif // ASYNCFUNCTION_H

