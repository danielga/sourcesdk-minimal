#pragma once

#include "tier1/interface.h"

#define INTERFACEVERSION_INTROSCREEN			"IntroScreen004"

class IMatRenderContext;

abstract_class IIntroScreen
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void Paint( const char *status, bool increment ) = 0;
	virtual void Draw( IMatRenderContext *pRenderContext, const char *status, int width, int height, float progress ) = 0;
};