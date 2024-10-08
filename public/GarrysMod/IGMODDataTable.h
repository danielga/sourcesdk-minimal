#pragma once

#include "CGMODVariant.h"

class IGMODDataTable
{
public:
	virtual void *GetKey( int ) = 0;
	virtual const CGMODVariant& GetValue( int ) = 0;
	virtual void IncrementIterator( int& ) = 0;
	virtual void *Get( int ) = 0; // Probably returns CGMODVariant
	virtual void Set( int, CGMODVariant const& ) = 0;
	virtual bool HasKey( int ) = 0;
	virtual const CGMODVariant& GetLocal( char const* ) = 0;
	virtual void SetLocal( char const*, CGMODVariant const& ) = 0;
	virtual void ClearLocal( char const* ) = 0;
	virtual void Clear() = 0;
	virtual void Begin() = 0;
	virtual void End() = 0;
};