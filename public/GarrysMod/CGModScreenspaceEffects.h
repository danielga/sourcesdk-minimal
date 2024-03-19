#pragma once

class KeyValues;

abstract_class CGModScreenspaceEffects
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void SetParameters( KeyValues* ) = 0;
	virtual void Render( int, int, int, int ) = 0;
	virtual void Enable( bool );
	virtual bool IsEnabled();
};