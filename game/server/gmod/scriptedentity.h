#pragma once

class CScriptedEntity
{
private:
	void* m_Offset[2];
	CBaseEntity* m_pEntity;
	CNetworkString( m_ScriptName, 64 );
}