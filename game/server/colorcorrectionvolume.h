//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Color correction entity.
//
// $NoKeywords: $
//=============================================================================//

#include <string.h>

#include "cbase.h"
#include "triggers.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//------------------------------------------------------------------------------
// FIXME: This really should inherit from something	more lightweight
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Purpose : Shadow control entity
//------------------------------------------------------------------------------
class CColorCorrectionVolume : public CBaseTrigger
{
	DECLARE_CLASS( CColorCorrectionVolume, CBaseTrigger );
public:
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CColorCorrectionVolume();

	void Spawn( void );
	bool KeyValue( const char *szKeyName, const char *szValue );
	int  UpdateTransmitState();

	void ThinkFunc();

	virtual bool PassesTriggerFilters(CBaseEntity *pOther);
	virtual void StartTouch( CBaseEntity *pEntity );
	virtual void EndTouch( CBaseEntity *pEntity );

	virtual int	ObjectCaps( void ) { return BaseClass::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	
	// Inputs
	void	InputEnable( inputdata_t &inputdata );
	void	InputDisable( inputdata_t &inputdata );

private:

	bool		m_bEnabled;
	bool		m_bStartDisabled;

	CNetworkVar( float, m_Weight );
	CNetworkVar( float, m_MaxWeight ); 
	CNetworkString( m_lookupFilename, MAX_PATH );

	float		m_LastEnterWeight;
	float		m_LastEnterTime;

	float		m_LastExitWeight;
	float		m_LastExitTime;

	float		m_FadeDuration;
};