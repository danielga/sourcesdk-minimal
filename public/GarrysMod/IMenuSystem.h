#pragma once

#include "tier1/interface.h"
#include "GarrysMod/Addon.h"

#define INTERFACEVERSION_MENUSYSTEM			"MenuSystem002"

class IGet;
class IGarrysMod;
class CGlobalVarsBase;
class INetworkStringTableContainer;

struct CLuaError;

abstract_class IMenuSystem
{
public:
	virtual int Init( CreateInterfaceFn, IGet*, IGarrysMod*, CGlobalVarsBase* ) = 0;
	virtual void Shutdown() = 0;
	virtual void SetupNetworkString( INetworkStringTableContainer* ) = 0;
	virtual void Think() = 0;
	virtual void StartLua() = 0;
	virtual void ServerDetails( const char*, const char*, const char*,int, const char* ) = 0;
	virtual void OnLuaError( CLuaError*, IAddonSystem::Information* ) = 0;
	virtual void SendProblemToMenu( const char* id, int severity, const char* params ) = 0;
	virtual bool IsServerBlacklisted( const char* address, const char* hostname, const char* description, const char* gamemode, const char* map ) = 0;
};