#if PLATFORM_64BITS
#include "mathlib/ssemath_x64.h"
#else
#include "mathlib/ssemath_x86.h"
#endif