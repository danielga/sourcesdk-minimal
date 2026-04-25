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

//-----------------------------------------------------------------------------
// Purpose: Dispatches explosion tempentity
//-----------------------------------------------------------------------------
class CTEExplosion : public CTEParticleSystem
{
public:
	DECLARE_CLASS( CTEExplosion, CTEParticleSystem );
	DECLARE_SERVERCLASS();

					CTEExplosion( const char *name );
	virtual			~CTEExplosion( void );

	virtual void	Test( const Vector& current_origin, const QAngle& current_angles );
	

public:
	CNetworkVar( float, m_fScale );
	CNetworkVar( int, m_nFlags );
	CNetworkVar( int, m_nRadius );
	CNetworkVar( int, m_nMagnitude );
};