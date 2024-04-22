#pragma once

#include "steam/isteamutils.h"

class IFileSystem;
class ILuaShared;
class ILuaConVars;
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

abstract_class IGet
{
public:
	virtual void OnLoadFailed( const char* reason ) = 0;
	virtual const char* GameDir() = 0;
	virtual bool IsDedicatedServer() = 0;
	virtual int GetClientCount() = 0;
	virtual IFileSystem* FileSystem() = 0;
	virtual ILuaShared* LuaShared() = 0;
	virtual ILuaConVars* LuaConVars() = 0;
	virtual IMenuSystem* MenuSystem() = 0;
	virtual IResources* Resources() = 0;
	virtual IIntroScreen* IntroScreen() = 0;
	virtual IMaterialSystem* Materials() = 0;
	virtual IGMHTML* HTML() = 0;
	virtual IServerAddons* ServerAddons() = 0;
	virtual ISteamHTTP* SteamHTTP() = 0;
	virtual ISteamRemoteStorage* SteamRemoteStorage() = 0;
	virtual ISteamUtils* SteamUtils() = 0;
	virtual ISteamApps* SteamApps() = 0;
	virtual ISteamScreenshots* SteamScreenshots() = 0;
	virtual ISteamUser* SteamUser() = 0;
	virtual ISteamFriends* SteamFriends() = 0;
	virtual ISteamUGC* SteamUGC() = 0;
	virtual ISteamGameServer* SteamGameServer() = 0;
	virtual ISteamNetworking* SteamNetworking() = 0;
	virtual void Initialize( IFileSystem* ) = 0;
	virtual void ShutDown() = 0;
	virtual void RunSteamCallbacks() = 0;
	virtual void ResetSteamAPIs() = 0;
	virtual void SetMotionSensor( IMotionSensor* ) = 0;
	virtual IMotionSensor* MotionSensor() = 0;
	virtual int Version() = 0;
	virtual const char* VersionStr() = 0;
	virtual IGMod_Audio* Audio() = 0;
	virtual const char* VersionTimeStr() = 0;
	virtual IAnalytics Analytics() = 0;
	virtual void UpdateRichPresense( const char* status ) = 0;
	virtual void ResetRichPresense() = 0;
	virtual void FilterText(const char*, char*, int, ETextFilteringContext, CSteamID) = 0;
};