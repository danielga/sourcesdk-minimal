#include "triggers.h"

class CSENT_brush : public CBaseTrigger
{
public:
	DECLARE_CLASS( CSENT_brush, CBaseTrigger );

	DECLARE_DATADESC();

	virtual void Spawn( void );
	virtual void Precache();
	virtual bool KeyValue( const char *szKeyName, const char *szValue );
	virtual bool AcceptInput( const char *szInputName, CBaseEntity *pActivator, CBaseEntity *pCaller, variant_t Value, int outputID );
	virtual void OnRestore();
	virtual void StartTouch( CBaseEntity *pOther );
	virtual void Touch( CBaseEntity *pOther ); 
	virtual void EndTouch( CBaseEntity *pOther );
	virtual void UpdateOnRemove( void );
	virtual bool UsesLua();
	virtual int GetLuaEntityType();
	virtual const char *GetLuaScriptName();
	virtual void InitializeScriptedEntity( const char * );
	virtual bool PassesTriggerFilters(CBaseEntity *pOther);

	virtual void ScriptThink();
	virtual void doKeyValue( char const*, char const* );
};