#pragma once

#include "tier1/interface.h"

#define INTERFACEVERSION_SERVERADDONS			"IServerAddons001"

abstract_class IServerAddons
{
public:
	virtual void Update() = 0;
	virtual int GetCount() = 0;
	virtual void Queue( const char* ) = 0;
	virtual void Clear() = 0;
	virtual void MountDownloadedAddons() = 0;
};