#pragma once

#include "igameevents.h"

abstract_class IGarrysMod : public IGameEventListener2
{
public:
	virtual void MenuThink() = 0;
	virtual void RunConsoleCommand( const char* cmd ) = 0;
	virtual void StartVideoScale( int, int ) = 0;
	virtual void EndVideoScale( int, int ) = 0;
};