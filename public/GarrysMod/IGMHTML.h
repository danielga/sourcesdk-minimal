#pragma once

#include "tier1/interface.h"

#define INTERFACEVERSION_GMHTML			"IGMHTML001"

class IGet;
namespace Awesomium
{
	class WebView;
}

abstract_class IGMHTML
{
public:
	virtual int Init( CreateInterfaceFn, IGet* ) = 0;
	virtual void Shutdown() = 0;
	virtual void Update() = 0;
	virtual Awesomium::WebView* GetWebView( ) = 0;
	virtual void KillWebView( Awesomium::WebView* ) = 0;
};