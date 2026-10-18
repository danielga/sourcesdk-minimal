#pragma once

#include "steam/isteamutils.h"

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaShared;
		class ILuaConVars;
	}
}

class IFileSystem;
class IMenuSystem;
class IResources;
class IIntroScreen;
class CIntroMenu;
class IMaterialSystem;
class IGMHTML;
class IServerAddons;
class ISteamHTTP;
class ISteamRemoteStorage;
class ISteamUtils;
class ISteamApps;
class ISteamScreenshots;
class ISteamUser;
class ISteamFriends;
class ISteamUGC;
class ISteamGameServer;
class ISteamNetworking;
class IMotionSensor;
class IGMod_Audio;
class IAnalytics;
class CSteamID;

class IGet
{
public:
	virtual void OnLoadFailed( const char *pszReason ) = 0;
	virtual const char* GameDir() = 0;
	virtual bool IsDedicatedServer() = 0;
	virtual int GetClientCount() = 0;
	virtual IFileSystem* FileSystem() = 0;
	virtual GarrysMod::Lua::ILuaShared* LuaShared() = 0;
	virtual GarrysMod::Lua::ILuaConVars* LuaConVars() = 0;
	virtual IMenuSystem* MenuSystem() = 0;
	virtual IResources* Resources() = 0;
	virtual IIntroScreen* IntroScreen() = 0;
	virtual IMaterialSystem* Materials() = 0;
	virtual IServerAddons* ServerAddons() = 0;
	virtual IGMHTML* HTML() = 0;
	virtual ISteamHTTP* SteamHTTP() = 0;
	virtual ISteamUtils* SteamUtils() = 0;
	virtual ISteamUGC* SteamUGC() = 0;
	virtual ISteamNetworking* SteamNetworking() = 0;
	virtual void Initialize( IFileSystem* ) = 0;
	virtual void ShutDown() = 0;
	virtual void RunSteamCallbacks() = 0;
	virtual void ResetSteamAPIs() = 0;
	virtual void SetMotionSensor( IMotionSensor* ) = 0;
	virtual IMotionSensor* MotionSensor() = 0;
	virtual int Version() = 0;
	virtual const char* VersionStr() = 0;
	virtual const char* SteamBranch() = 0;
	virtual IGMod_Audio* Audio() = 0;
	virtual const char* VersionTimeStr() = 0;
	virtual IAnalytics Analytics() = 0;
	virtual void UpdateRichPresense( const char* status ) = 0;
	virtual void ResetRichPresense() = 0;
	virtual const char* BaseDir() = 0; // ToDo: Check if this is dedicated server only
	virtual void FilterText(const char*, char*, int, ETextFilteringContext, CSteamID) = 0;
};

extern IGet* get;