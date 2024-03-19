#pragma once

#include "igameevents.h"

abstract_class IGarrysMod : public IGameEventListener2
{
public:
	virtual void MD5String( char* out, const char*, const char*, const char* ) = 0;
	virtual void PlaySound( const char* sound ) = 0;
	virtual const char* GetMapName() = 0;
	virtual void RunConsoleCommand( const char* cmd ) = 0;
	virtual void StartVideoScale( int, int ) = 0;
	virtual void EndVideoScale( int, int ) = 0;
};