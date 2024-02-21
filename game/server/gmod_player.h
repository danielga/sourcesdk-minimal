#ifndef GMOD_PLAYER_H
#define GMOD_PLAYER_H
#pragma once

class CGMOD_Player;

#include "hl2mp/hl2mp_player.h"

class CGMOD_Player : public CHL2MP_Player
{
public:
	DECLARE_CLASS( CGMOD_Player, CHL2MP_Player );

	CGMOD_Player();
	~CGMOD_Player( void );

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual int GiveAmmo( int nCount, int nAmmoIndex, bool bSuppressSound );
	virtual void Weapon_Equip( CBaseCombatWeapon *pWeapon );
	virtual void Weapon_Drop( CBaseCombatWeapon *pWeapon, const Vector *pvecTarget = NULL, const Vector *pVelocity = NULL );
	virtual void CreateViewModel( int viewmodelindex = 0 );
	virtual void SetupVisibility( CBaseEntity *pViewEntity, unsigned char *pvs, int pvssize );
	virtual void InitialSpawn();
	virtual void PlayerDeathThink( void );
	virtual void PreThink();
	virtual void PostThink();
	virtual void OnDamagedByExplosion( const CTakeDamageInfo &info );
	virtual void PlayStepSound( Vector &vecOrigin, surfacedata_t *psurface, float fvol, bool force );
	virtual void DeathSound( const CTakeDamageInfo &info );
	virtual void SetAnimation( PLAYER_ANIM playerAnim );
	virtual bool ClientCommand( const CCommand &args );
	virtual bool StartObserverMode( int mode );
	virtual CBaseEntity* EntSelectSpawnPoint( void );
	virtual bool GetInVehicle( IServerVehicle *pVehicle, int nRole );
	virtual bool BumpWeapon( CBaseCombatWeapon *pWeapon );
	virtual void PlayerUse( void );
	virtual CBaseEntity *FindUseEntity( void );
	virtual void PickupObject ( CBaseEntity *pObject, bool bLimitMassAndSize );
	virtual void PlayerRunCommand( CUserCmd *ucmd, IMoveHelper *moveHelper );

	virtual float GetFOV( void );
	virtual int GetFOVForNetworking( void );
	virtual void UpdateButtonState( int nUserCmdButtonMask );
	virtual void OnPlayerSay( const char* );
	virtual float RestrictPlayerPitch();
	virtual bool CanAttack();
	virtual int GetMaxArmor();

	virtual void StartSprinting( void );
	virtual void StopSprinting( void );
	virtual bool CanSprint( void );

	virtual void HandleSpeedChanges( void );
	virtual void GiveAllItems( void );
	virtual void SetPlayerColor( Vector color );
	virtual void SetWeaponColor( Vector color );
};

inline CGMOD_Player *ToCGMOD_Player( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<CGMOD_Player*>( pEntity );
}

#endif //GMOD_PLAYER_H