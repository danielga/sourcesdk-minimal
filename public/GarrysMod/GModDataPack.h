#pragma once

#include <vector>
#include <string>

class LuaFindResult;

namespace GarrysMod::Lua
{
	struct LuaFile; // RaphaelIT7: I'm guessing that GetFromDatatable & FindFileInDatatable return this struct(defined in garrysmod_common -> LuaShared.h) but I dind't verify it yet.
}

abstract_class GModDataPack
{
public:
	virtual void* GetFromDatatable( const std::string& ) = 0;
	virtual void* GetHashFromDatatable( const std::string& ) = 0;
	virtual void* GetHashFromString( const char*, unsigned long ) = 0;
	virtual void FindInDatatable( const std::string&, std::vector<LuaFindResult>&, bool ) = 0;
	virtual void* FindFileInDatatable( const std::string&, bool, bool ) = 0;
	virtual bool IsSingleplayer() = 0;
	virtual void UnknownMethod() = 0; // Name in Gmod: v000000000000000000000000000oo0000000000000000fff000000000000000000000000000000000o0o
	virtual bool IsValidDirectory( const std::string& ) = 0;
};