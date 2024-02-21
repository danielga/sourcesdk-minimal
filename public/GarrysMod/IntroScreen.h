#pragma once

#include "tier1/interface.h"

#define INTERFACEVERSION_INTROSCREEN			"IntroScreen004"

class MatRenderContextPtr;

abstract_class IIntroScreen
{
public:
	virtual void Start() = 0;
	virtual void End() = 0;
	virtual void Update( const char*, bool ) = 0;
	virtual void DoDraw( MatRenderContextPtr&, const char*, int, int, float ) = 0;
};