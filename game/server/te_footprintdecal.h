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
// Purpose: Dispatches footprint decal tempentity
//-----------------------------------------------------------------------------

#define FOOTPRINT_DECAY_TIME 3.0f

class CTEFootprintDecal : public CBaseTempEntity
{
public:
	DECLARE_CLASS( CTEFootprintDecal, CBaseTempEntity );

					CTEFootprintDecal( const char *name );
	virtual			~CTEFootprintDecal( void );

	DECLARE_SERVERCLASS();

public:
	CNetworkVector( m_vecOrigin );
	CNetworkVector( m_vecDirection );											
	CNetworkVar( int, m_nEntity );
	CNetworkVar( int, m_nIndex );
};