//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: spherical math routines
//
//=====================================================================================//

#include <math.h>
#include <float.h>	// Needed for FLT_EPSILON
#include "basetypes.h"
#include <memory.h>
#include "tier0/dbg.h"
#include "mathlib/mathlib.h"
#include "mathlib/vector.h"
#include "mathlib/spherical_geometry.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

float s_flFactorials[]={
	1.f,
	1.f,
	2.f,
	6.f,
	24.f,
	120.f,
	720.f,
	5040.f,
	40320.f,
	362880.f,
	3628800.f,
	39916800.f,
	479001600.f,
	6227020800.f,
	87178291200.f,
	1307674368000.f,
	20922789888000.f,
	355687428096000.f,
	6402373705728000.f,
	121645100408832000.f,
	2432902008176640000.f,
	51090942171709440000.f,
	1124000727777607680000.f,
	25852016738884976640000.f,
	620448401733239439360000.f,
	15511210043330985984000000.f,
	403291461126605635584000000.f,
	10888869450418352160768000000.f,
	304888344611713860501504000000.f,
	8841761993739701954543616000000.f,
	265252859812191058636308480000000.f,
	8222838654177922817725562880000000.f,
	263130836933693530167218012160000000.f,
	8683317618811886495518194401280000000.f
};

float AssociatedLegendrePolynomial( int nL, int nM, float flX )
{
	// evaluate associated legendre polynomial at flX, using recurrence relation
	float flPmm = 1.;
	if ( nM > 0 )
	{
		float flSomX2 = sqrt( ( 1 - flX ) * ( 1 + flX ) );
		float flFact = 1.;
		for( int i = 0 ; i < nM; i++ )
		{
			flPmm *= -flFact * flSomX2;
			flFact += 2.0;
		}
	}
	if ( nL == nM )
		return flPmm;
	float flPmmp1 = flX * ( 2.0 * nM + 1.0 ) * flPmm;
	if ( nL == nM + 1 ) 
		return flPmmp1;
	float flPll = 0.;
	for( int nLL = nM + 2 ; nLL <= nL; nLL++ )
	{
		flPll = ( ( 2.0 * nLL - 1.0 ) * flX * flPmmp1 - ( nLL + nM - 1.0 ) * flPmm ) * ( 1.0 / ( nLL - nM ) );
		flPmm = flPmmp1;
		flPmmp1 = flPll;
	}
	return flPll;
}

static float SHNormalizationFactor( int nL, int nM )
{
	double flTemp = ( ( 2. * nL + 1.0 ) * s_flFactorials[ nL - nM ] )/ ( 4. * M_PI * s_flFactorials[ nL + nM ] );
	return sqrt( flTemp );
}

#define SQRT_2 1.414213562373095 

FORCEINLINE float SphericalHarmonic( int nL, int nM, float flTheta, float flPhi, float flCosTheta )
{
	if ( nM == 0 )
		return SHNormalizationFactor( nL, 0 ) * AssociatedLegendrePolynomial( nL, nM, flCosTheta );

	if ( nM > 0 )
		return SQRT_2 * SHNormalizationFactor( nL, nM ) * cos ( nM * flPhi ) *
			AssociatedLegendrePolynomial( nL, nM, flCosTheta );

	return 
		SQRT_2 * SHNormalizationFactor( nL, -nM ) * sin( -nM * flPhi ) * AssociatedLegendrePolynomial( nL, -nM, flCosTheta );

}

float SphericalHarmonic( int nL, int nM, float flTheta, float flPhi )
{
	return SphericalHarmonic( nL, nM, flTheta, flPhi, cos( flTheta ) );
}

float SphericalHarmonic( int nL, int nM, Vector const &vecDirection )
{
	Assert( fabs( VectorLength( vecDirection ) - 1.0 ) < 0.0001 );
	float flPhi = acos( vecDirection.z );
	float flTheta = 0;
	float S = Square( vecDirection.x ) + Square( vecDirection.y );
	if ( S > 0 )
	{
		flTheta = atan2( vecDirection.y, vecDirection.x );
	}
	return SphericalHarmonic( nL, nM, flTheta, flPhi, cos( flTheta ) );
}

