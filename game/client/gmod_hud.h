#pragma once

#include "hud.h"

class PanelAnimationMap;

namespace vgui
{
	class PanelMessageMap;
	class PanelKeyBindingMap;
}

abstract_class CHudGMod : public CHudElement
{
public:
	virtual void Init();
	virtual void VidInit();
	virtual void Reset();

	virtual vgui::PanelMessageMap *GetMessageMap();
	virtual PanelAnimationMap *GetAnimMap();
	virtual vgui::PanelKeyBindingMap *GetKBMap();
	virtual void OnThink();
	virtual void Paint();
	virtual void PaintBackground();
};