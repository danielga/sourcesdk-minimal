#include "baseentity.h"

class CSENT_point : public CBaseEntity
{
public:
	DECLARE_CLASS( CSENT_point, CBaseEntity );

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual int UpdateTransmitState( void );
	virtual void Spawn( void );
	virtual void Precache();
	virtual bool KeyValue( const char *szKeyName, const char *szValue );
	virtual int ObjectCaps(void);
	virtual bool AcceptInput( const char *szInputName, CBaseEntity *pActivator, CBaseEntity *pCaller, variant_t Value, int outputID );
	virtual void OnRestore();
	virtual void UpdateOnRemove( void );
	virtual bool UsesLua();
	virtual int GetLuaEntityType();
	virtual const char *GetLuaScriptName();
	virtual void InitializeScriptedEntity( const char * );

	virtual void ScriptThink();
	virtual void ClientThink();
};