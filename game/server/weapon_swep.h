#include "hl2mp/weapon_hl2mpbasehlmpcombatweapon.h"

class CWeaponSWEP : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponSWEP, CBaseHL2MPCombatWeapon );

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC(); 

	virtual void Spawn( void );
	virtual void Precache( void );
	virtual bool KeyValue( const char *szKeyName, const char *szValue );
	virtual bool AcceptInput( const char *szInputName, CBaseEntity *pActivator, CBaseEntity *pCaller, variant_t Value, int outputID );
	virtual void OnRestore();
	virtual void UpdateOnRemove( void );
	virtual void DoImpactEffect( trace_t &tr, int nDamageType );
	virtual void OnOwnerChanged();
	virtual bool UsesLua();
	virtual int GetLuaEntityType();
	virtual const char *GetLuaScriptName();
	virtual void InitializeScriptedEntity( const char * );
	virtual void Lua_OnEntityInitialized();
	virtual void Equip( CBaseCombatCharacter *pOwner );
	virtual void Drop( const Vector &vecVelocity );
	virtual bool Deploy( void );
	virtual bool Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	virtual void ItemPostFrame( void );
	virtual void ItemBusyFrame( void );
	virtual bool Reload( void );
	virtual void PrimaryAttack( void );
	virtual void SecondaryAttack( void );
	virtual Vector GetBulletSpread( WeaponProficiency_t proficiency );
	virtual float GetFireRate( void );
	virtual int GetMinBurst();
	virtual int GetMaxBurst();
	virtual float GetMinRestTime();
	virtual float GetMaxRestTime();
	virtual bool CanBePickedUpByNPCs( void );
	virtual const char *GetViewModel( int viewmodelindex = 0 ) const;
	virtual const char *GetWorldModel( void ) const;
	virtual int GetMaxClip1( void ) const;
	virtual int GetMaxClip2( void ) const;
	virtual int GetDefaultClip1( void ) const;
	virtual int GetDefaultClip2( void ) const;
	virtual int GetWeight( void ) const;
	virtual bool AllowsAutoSwitchTo( void ) const;
	virtual bool AllowsAutoSwitchFrom( void ) const;
	virtual int GetSlot( void ) const;
	virtual int GetPosition( void ) const;
	virtual char const *GetName( void ) const;
	virtual char const *GetPrintName( void ) const;
	virtual char const *GetHoldType();
	virtual char const *SetHoldType( char const* );
	virtual Activity ActivityOverride( Activity baseAct, bool *pRequired );
	virtual int CapabilitiesGet( void );
	virtual void Operator_HandleAnimEvent( animevent_t *pEvent, CBaseCombatCharacter *pOperator );
	virtual float GetPlayerDamage( void );
	virtual void EquipAmmo( CBaseEntity* );
	virtual bool ShouldDropOnDie( void );
	virtual bool ShouldDrawViewModel( void );
	virtual float GetFOV( float );
	virtual void OnRangeAttack1( void );
	virtual void GModNPCAttackHack( void );

	virtual void doKeyValue( char const*, char const* );
	virtual void UpdateLuaData( bool );
	virtual bool GetPrimaryAutomatic( void );
	virtual bool GetSecondaryAutomatic( void );
	virtual void UpdateHoldType( void );
};