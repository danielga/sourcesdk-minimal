#pragma once

#include <vector>
#include <string>

class LuaFindResult;

abstract_class GModDataPack
{
public:
	virtual void* GetFromDatatable( std::string const& ) = 0;
	virtual void* GetHashFromDatatable( std::string const& ) = 0;
	virtual void* GetHashFromString( char const*, unsigned long ) = 0;
	virtual void FindInDatatable( std::string const&, std::vector<LuaFindResult>&, bool ) = 0;
	virtual void* FindFileInDatatable( std::string const&, bool, bool ) = 0;
	virtual bool IsSingleplayer() = 0;
	virtual void UnknownMethod() = 0; // Name in Gmod: v000000000000000000000000000oo0000000000000000fff000000000000000000000000000000000o0o
};