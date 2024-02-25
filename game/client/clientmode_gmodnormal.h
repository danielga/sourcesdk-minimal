#pragma once

#include "clientmode_shared.h"

class ClientModeGModNormal : public ClientModeShared
{
public:
	virtual ~ClientModeGModNormal();
	virtual Init();
	virtual KeyInput( int, ButtonCode_t, char const* );
};