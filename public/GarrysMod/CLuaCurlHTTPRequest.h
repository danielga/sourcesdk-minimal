#pragma once

class ILuaInterface;

namespace GarrysMod::Lua {
	class ILuaBase;
}

abstract_class CLuaCurlHTTPRequest
{
public:
	virtual ~CLuaCurlHTTPRequest() = 0;
	virtual void Run() = 0;
	virtual void OnThreadFinished() = 0;
	virtual bool IsFinished() = 0;
	virtual void DoFinish(GarrysMod::Lua::ILuaBase*) = 0;
	virtual void DestroyForced() = 0;
};