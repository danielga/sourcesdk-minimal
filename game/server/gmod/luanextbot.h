#include "nextbot/nextbot.h"

class LuaNextBot : public NextBotCombatCharacter, CScriptedEntity
{
private:
	CNetworkVar( unsigned char, m_lifeState ); // RaphaelIT7: GMod uses 3 bits for networking
}