#pragma once

class ILuaInterface;

abstract_class CLuaUser
{
public:
	virtual ~CLuaUser() = 0;
	virtual bool IsUsingLua() = 0;
	virtual void InitLibraries(ILuaInterface*);
};