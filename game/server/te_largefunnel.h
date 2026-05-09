//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#pragma once

#include "te_particlesystem.h"

// dimhotepus: short -> int.
extern int	g_sModelIndexSmoke;			// (in combatweapon.cpp) holds the index for the smoke cloud

//-----------------------------------------------------------------------------
// Purpose: Dispatches smoke tempentity
//-----------------------------------------------------------------------------
class CTELargeFunnel : public CTEParticleSystem
{
public:
	DECLARE_CLASS( CTELargeFunnel, CTEParticleSystem );
	DECLARE_SERVERCLASS();

					CTELargeFunnel( const char *name );
	virtual			~CTELargeFunnel( void );

	virtual void	Test( const Vector& current_origin, const QAngle& current_angles );
	
public:
	CNetworkVar( int, m_nReversed );
};