#pragma once

#include <string>
#include <list>
#include <cstdint>

class IGamemodeSystem
{
public:
	struct Information
	{
		bool exists;
		bool menusystem;
		std::string title;
		std::string name;
		std::string maps;
		std::string basename;
		std::string category;
		uint64_t workshopid;
	};

	virtual void OnJoinServer( const std::string &strGamemode ) = 0;
	virtual void OnLeaveServer() = 0;
	virtual void Refresh() = 0;
	virtual void Clear() = 0;
	virtual const IGamemodeSystem::Information &Active() = 0;
	// The exists field will be false if none was found!
	virtual const IGamemodeSystem::Information &FindByName( const std::string &strGamemode ) = 0;
	virtual void SetActive( const std::string &strGamemode ) = 0;
	virtual const std::list<IGamemodeSystem::Information> &GetList() const = 0;
	virtual bool IsServerBlacklisted( char const* address, char const* hostname, char const* description, char const* gm, char const* map ) = 0;
	virtual void OnServerDownloadsMounted() = 0;
};