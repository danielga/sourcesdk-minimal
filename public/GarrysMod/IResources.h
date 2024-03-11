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

abstract_class CVideoWriter
{
public:
	virtual void Start( char const*, char const*, char const*, int, int, int, char const*, int, float, bool ) = 0;
	virtual void AddFrame( int, int, float ) = 0;
	virtual void Finish() = 0;
	virtual void Delete() = 0;
	virtual void ManualFiling() = 0;
};

abstract_class CVideoHolly : CVideoWriter
{
public:
	virtual void AddAudio( void*, uint, unsigned char, unsigned char ) = 0;
	virtual void StartMovie() = 0;
	virtual void EncodeRGB( void*, float ) = 0;
	virtual void EndMovie() = 0;
	virtual void ManualFiling() = 0;
	virtual const char* FileExtension() = 0;
};

abstract_class IResources
{
public:
	virtual int Init( CreateInterfaceFn ) = 0;
	virtual void Shutdown() = 0;
	virtual CVideoHolly* CreateMovie() = 0;
	virtual IMaterial* FindMaterial( const char*, const char*, bool, bool, bool ) = 0;
	virtual Color GetTextureColour( ITexture*, int, int ) = 0;
	virtual void SavePNG( int, int, unsigned char*, const char*, int, int ) = 0;
	virtual void SaveJPG( int, int, int, unsigned char*, const char*, int, int, CUtlBuffer* ) = 0;
	virtual bool ShouldRecordSound() = 0;
	virtual void AudioSamples( void*, uint, unsigned char, unsigned char ) = 0;
	virtual void SavePNGToBuffer( int, int, unsigned char*, Bootil::Buffer&, int, int ) = 0;
	virtual void SaveJPGToBuffer( int, int, unsigned char*, Bootil::Buffer&, int, int, int ) = 0;
	virtual void SetImage( ITexture*, const char* ) = 0;
};