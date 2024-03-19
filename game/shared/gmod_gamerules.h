#ifndef GMOD_GAMERULES_H
#define GMOD_GAMERULES_H
#pragma once

#include "gamerules.h"
#include "hl2mp/hl2mp_gamerules.h"

#ifdef CLIENT_DLL

	#define CGMODRules C_GMODRules

#else

	#include "takedamageinfo.h"

#endif


//
// gmod_gamerules.h
//

class CGMODRules : public CHL2MPRules
{
public:
	DECLARE_CLASS( CGMODRules, CHL2MPRules );

	virtual void LevelInitPreEntity();

	CGMODRules();
	virtual ~CGMODRules() {};

	virtual bool ShouldCollide( int, int );
	virtual const unsigned char* GetEncryptionKey();
	virtual void Precache();
	virtual void Think();
	virtual void OnSkillLevelChanged( int );
	virtual bool IsTeamplay();
	virtual const char* GetGameDescription();
	virtual void ClientDisconnected( edict_t *pClient );
	virtual float FlPlayerFallDamage( CBasePlayer *pPlayer );
	virtual bool FPlayerCanTakeDamage( CBasePlayer *pPlayer, CBaseEntity *pAttacker, const CTakeDamageInfo &info );
	virtual bool ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target );
	virtual bool FlPlayerFallDeathDoesScreenFade( CBasePlayer *pl );
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );
	virtual void DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	virtual bool CanHavePlayerItem( CBasePlayer *pPlayer, CBaseCombatWeapon *pItem );
	virtual bool CanHaveItem( CBasePlayer *pPlayer, CItem *pItem );
	virtual int ItemShouldRespawn( CItem *pItem );
	virtual void InitDefaultAIRelationships();
	virtual const char* AIClassText( int classType );
	virtual void CreateStandardEntities();
	virtual bool ShouldBurningPropsEmitLight();
	virtual void CleanUpMap();
};

inline CGMODRules* GMODGameRules()
{
	return static_cast<CGMODRules*>(g_pGameRules);
}

#endif // GMOD_GAMERULES_H
