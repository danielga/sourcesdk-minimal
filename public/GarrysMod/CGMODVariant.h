#pragma once

/*
	Types (Type - Number - Conversion):
	NIL = 0,
	Number = 1, - *((float*)&value.data)
	Unknown = 2, - Unknown
	Bool = 3, - (bool)value.data
	Vector = 4, - (Vector&)value.data
	Angle = 5, - (QAngle&)value.data
	Entity = 6, - Unknown
	String = 7  - str = (const char*)value.data2 - length = (int)value.data
*/

struct CGMODVariant
{
	unsigned char type; // Used by Push_GMODVariant to determen which function to use to push it.
	void* data; // Can be a bool, int, float, Vector&, QAngle&
	void* data2; // Can be a const char* or a float. If the type is a string, data will be the strings length.
	void* data3; // Will be a float. Only used by Vector and QAngle for the third value.
};