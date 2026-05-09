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

#include "basetempentity.h"

//-----------------------------------------------------------------------------
// Purpose: Display's a blood sprite
//-----------------------------------------------------------------------------
class CTEBloodSprite : public CBaseTempEntity
{
public:
	DECLARE_CLASS( CTEBloodSprite, CBaseTempEntity );

					CTEBloodSprite( const char *name );
	virtual			~CTEBloodSprite( void );

	virtual void	Test( const Vector& current_origin, const QAngle& current_angles );

	DECLARE_SERVERCLASS();

public:
	CNetworkVector( m_vecOrigin );
	CNetworkVector( m_vecDirection );
	CNetworkVar( int, r );
	CNetworkVar( int, g );
	CNetworkVar( int, b );
	CNetworkVar( int, a );
	CNetworkVar( int, m_nSize );
};