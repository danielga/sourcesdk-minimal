#pragma once

/*
	Types:
	NIL = 0
	Number = 1
	Int = 2
	Bool = 3
	Vector = 4
	Angle = 5,
	Entity = 6
	String = 7
*/

#define CGMODVariant_NIL 0
#define CGMODVariant_Number 1
#define CGMODVariant_Int 2
#define CGMODVariant_Bool 3
#define CGMODVariant_Vector 4
#define CGMODVariant_Angle 5
#define CGMODVariant_Entity 6
#define CGMODVariant_String 7
#define CGMODVariant_Count 8

class CBaseHandle;
struct CGMODVariant // NOTE: It doesn't seem work perfectly with GMOD(I hate m_Ent)
{
	unsigned char type = 0; // Used by Push_GMODVariant to determen which function to use to push it.
	union {
		struct {
			int m_Length;
			char* m_pString;
		};
		bool m_Bool;
		float m_Float;
		int m_Int;
		short m_Ent;
		Vector m_Vec;
		QAngle m_Ang;
	};
};