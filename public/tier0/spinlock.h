//========= Copyright © Valve Corporation, All rights reserved. ============//
#pragma once

// Enter Spin-lock: block until the specified bit(s) is(are) cleared, set it(them) and return the value of *pLock right before the lock has been acquired
template <typename T>
inline T EnterBitSpinLock( volatile T *pLock, T nSetFlag, int nSpinLock = 100 )
{
	T nOriginal;
	int nSpinCounter = 0;
	do
	{
		for(;; )
		{
			nOriginal = *pLock;
			if ( nOriginal & nSetFlag )
			{
				// another thread locked this spinlock, wait
				if ( ++nSpinCounter % nSpinLock )
					ThreadPause();
				else
					ThreadSleep( 0 );
			}
			else
			{
				// we're about to lock this flag
				break;
			}
		}

		// the original value doesn't hav the flag set - set that flag
	} while ( nOriginal != ThreadInterlockedCompareExchange( pLock, nOriginal | nSetFlag, nOriginal ) );

	// Sergiy: for a strictly safe lockless algorithm, we need release-acquire semantics here, but I don't expect anyone to observe it and I don't believe we're planning to be on ARM or other non-Intel platforms, so for now I'll skip it (otherwise acquire would be right in 99% of use cases)

	return nOriginal;
}



template <typename T>
inline T ThreadInterlockedAndNot( volatile T *pLock, T nResetFlags )
{
	T nOriginal;
	for(;; )
	{
		nOriginal = *pLock;
		T nNewValue = nOriginal & ~nResetFlags;
		if ( nOriginal == nNewValue
			 || nOriginal == ThreadInterlockedCompareExchange( pLock, nNewValue, nOriginal ) )
			break;

	};

	return nOriginal;
}

template <typename T>
inline T ThreadInterlockedOr( volatile T *pLock, T nMergeFlags )
{
	T nOriginal;
	for ( ;; )
	{
		nOriginal = *pLock;
		T nNewValue = nOriginal | nMergeFlags;
		if ( nOriginal == nNewValue
			 || nOriginal == ThreadInterlockedCompareExchange( pLock, nNewValue, nOriginal ) )
			break;

	};

	return nOriginal;
}

// Leave Spin-lock entered into earlier, presumably in the same thread. The resetflags can be more flags to reset than the original. There's no sleep-waiting here.
template <typename T>
inline T LeaveBitSpinLock( volatile T *pLock, T nResetFlags )
{
	// Sergiy: for a strictly safe lockless algorithm, we need release-acquire semantics here, but I don't expect anyone to observe it and I don't believe we're planning to be on ARM or other non-Intel platforms, so I'll use a memory barrier here (otherwise release is right in 99% of use cases)
	ThreadMemoryBarrier();

	return ThreadInterlockedAndNot( pLock, nResetFlags );
}



template <typename T>
inline void InterlockedIterate( volatile uint32 *pCounter, uint32 nTargetCount, T*pThis, void ( T::*pfnBody )( uint nIndex ) )
{
	for ( ; ; )
	{
#if 1
		uint32 nIndex = *pCounter;
		if ( nIndex >= nTargetCount )
			break;
		if ( ThreadInterlockedAssignIf( pCounter, nIndex + 1, nIndex ) )
			( pThis->*pfnBody )( nIndex );

#else
		uint32 nNewCounter = ThreadInterlockedIncrement( pCounter );
		uint32 nIndex = nNewCounter - 1;
		if ( nIndex < nTargetCount )
			( pThis->*pfnBody )( nIndex );
		else
			break;
#endif

	}
}
