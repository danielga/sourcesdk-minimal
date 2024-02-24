#include "baseanimating.h"

class CSENT_anim : public CBaseAnimating
{
public:
	DECLARE_CLASS( CSENT_anim, CBaseAnimating );

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual bool TestCollision( const Ray_t &ray, unsigned int mask, trace_t& trace );
	virtual int UpdateTransmitState( void );
	virtual void Spawn( void );
	virtual bool KeyValue( const char *szKeyName, const char *szValue );
	virtual int ObjectCaps(void);
	virtual bool AcceptInput( const char *szInputName, CBaseEntity *pActivator, CBaseEntity *pCaller, variant_t Value, int outputID );
	virtual void OnRestore();
	virtual int OnTakeDamage( const CTakeDamageInfo &info );
	virtual void ImpactTrace( trace_t *pTrace, int iDamageType, const char *pCustomImpactName = NULL );
	virtual void StartTouch( CBaseEntity *pOther );
	virtual void Touch( CBaseEntity *pOther ); 
	virtual void EndTouch( CBaseEntity *pOther );
	virtual void Blocked( CBaseEntity *pOther );
	virtual void UpdateOnRemove( void );
	virtual void DoImpactEffect( trace_t &tr, int nDamageType );

	CNetworkVarForDerived( int, m_iMaxHealth );
	CNetworkVarForDerived( int, m_iHealth );

	virtual void VPhysicsUpdate( IPhysicsObject *pPhysics );
	virtual void VPhysicsCollision( int index, gamevcollisionevent_t *pEvent );
	virtual	CBasePlayer *HasPhysicsAttacker( float dt );
	virtual bool UsesLua();
	virtual int GetLuaEntityType();
	virtual void StartMotionController();
	virtual void StopMotionController();
	virtual void AttachObjectToMotionController( IPhysicsObject * );
	virtual void DetachObjectFromMotionController( IPhysicsObject * );
	virtual void SetUseType( int );
	virtual const char *GetLuaScriptName();
	virtual void InitializeScriptedEntity( const char * );
	virtual void SetPhysicsAttacker( CBasePlayer*, float );
	virtual void HandleAnimEvent( animevent_t *pEvent );

	virtual void ScriptThink();
	virtual void CreateBoneFollowers();
	virtual void UpdateBoneFollowers();
	virtual void DestroyBoneFollowers();
	virtual simresult_e Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular );
	virtual void ScriptUse( CBaseEntity*, CBaseEntity*, USE_TYPE, float );
	virtual void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );
	virtual void OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t reason );
	virtual QAngle PreferredCarryAngles();
	virtual bool HasPreferredCarryAnglesForPlayer( CBasePlayer *pPlayer );
};