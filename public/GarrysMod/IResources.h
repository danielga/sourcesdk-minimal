#pragma once

#include "tier1/interface.h"

#define INTERFACEVERSION_RESOURCES			"IResources001"

class ITexture;
class CUtlBuffer;
class Color;

namespace Bootil
{
	class Buffer;
}

abstract_class IResources
{
public:
	virtual int Init( CreateInterfaceFn ) = 0;
	virtual void Shutdown() = 0;
	virtual void CreateMovie() = 0;
	virtual IMaterial* FindMaterial( const char*, const char*, bool, bool, bool ) = 0;
	virtual Color GetTextureColour( ITexture*, int, int ) = 0;
	virtual void SavePNG( int, int, uchar*, const char*, int, int ) = 0;
	virtual void SaveJPG( int, int, int, uchar*, const char*, int, int, CUtlBuffer* ) = 0;
	virtual bool ShouldRecordSound() = 0;
	virtual void AudioSamples( void*, uint, uchar, uchar ) = 0;
	virtual void SavePNGToBuffer( int, int, uchar*, Bootil::Buffer&, int, int ) = 0;
	virtual void SaveJPGToBuffer( int, int, uchar*, Bootil::Buffer&, int, int, int ) = 0;
	virtual void SetImage( ITexture*, const char* ) = 0;
};