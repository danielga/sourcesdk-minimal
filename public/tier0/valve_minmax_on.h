//========= Copyright Valve Corporation, All rights reserved. ============//
#if !defined(POSIX)
#ifndef vmin
	#define vmin(a,b)  (((a) < (b)) ? (a) : (b))
#endif
#ifndef vmax
	#define vmax(a,b)  (((a) > (b)) ? (a) : (b))
#endif
#endif
