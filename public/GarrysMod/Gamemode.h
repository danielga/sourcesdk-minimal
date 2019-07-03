#pragma once

#include <string>
#include <list>

namespace IGamemodeSystem
{

struct Information
{
	bool placeholder1;
	bool menusystem;
	std::string title;
	std::string name;
	std::string maps;
	std::string basename;
	std::string workshopid;
};

}

namespace Gamemode
{

class System
{
public:
	virtual void OnJoinServer( const std::string & ) = 0;
	virtual void OnLeaveServer( ) = 0;
	virtual void Refresh( ) = 0;
	virtual void Clear( ) = 0;
	virtual const IGamemodeSystem::Information &Active( ) = 0;
	virtual void FindByName( const std::string & ) = 0;
	virtual void SetActive( const std::string & ) = 0;
	virtual const std::list<IGamemodeSystem::Information> &GetList( ) const = 0;
};

}
