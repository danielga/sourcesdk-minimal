#include "filters.h"

class CSENT_filter : public CBaseFilter
{
public:
	DECLARE_CLASS( CSENT_filter, CBaseFilter );

	DECLARE_DATADESC();

	virtual int UpdateTransmitState( void );
	virtual void Spawn( void );
	virtual void Precache();
	virtual bool KeyValue( const char *szKeyName, const char *szValue );
	virtual bool AcceptInput( const char *szInputName, CBaseEntity *pActivator, CBaseEntity *pCaller, variant_t Value, int outputID );
	virtual void OnRestore();
	virtual void UpdateOnRemove( void );
	virtual bool UsesLua();
	virtual int GetLuaEntityType();
	virtual const char *GetLuaScriptName();
	virtual void InitializeScriptedEntity( const char * );

	virtual bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity );
	virtual bool PassesDamageFilterImpl(const CTakeDamageInfo &info);

	virtual void ScriptThink();
	virtual void ClientThink();
};