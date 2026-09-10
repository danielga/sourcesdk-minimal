//===== Copyright � 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: An extra interface implemented by the material system 
// implementation of vgui::ISurface
//
// $Revision: $
// $NoKeywords: $
//===========================================================================//

#ifndef IMATSYSTEMSURFACE_H
#define IMATSYSTEMSURFACE_H
#ifdef _WIN32
#pragma once
#endif


#include <vgui/vgui.h>
#include "vgui/ISurface.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class VMatrix;
class IMaterial;
struct InputEvent_t;


//-----------------------------------------------------------------------------
// Callbacks for mouse getting + setting
//-----------------------------------------------------------------------------
typedef void (*GetMouseCallback_t)(int &x, int &y);
typedef void (*SetMouseCallback_t)(int x, int y);

//-----------------------------------------------------------------------------
// Callbacks for sound playing
//-----------------------------------------------------------------------------
typedef void (*PlaySoundFunc_t)(const char *pFileName);


//-----------------------------------------------------------------------------
//
// An extra interface implemented by the material system implementation of vgui::ISurface
//
//-----------------------------------------------------------------------------
class IMatSystemSurface : public vgui::ISurface
{
public:
	virtual void AttachToWindow( void *hwnd, bool bLetAppDriveInput = false ) = 0;

	// Tells the surface to ignore windows messages
	virtual void EnableWindowsMessages( bool bEnable ) = 0;

	// Starts, ends 3D painting
	// NOTE: These methods should only be called from within the paint()
	// method of a panel.
	// When manual supersampling of rendertarget is enabled then the render
	// target will double the width and height of the rendering region if possible, or will otherwise
	// increase the rendering region to fill up the width/height of the render target maintaining
	// the aspect ratio.
	virtual void Begin3DPaint( int iLeft, int iTop, int iRight, int iBottom, bool bSupersampleRT = false ) = 0;
	virtual void End3DPaint( bool bIgnoreAlphaWhenCompositing = false ) = 0;

	// NOTE: This also should only be called from within the paint()
	// method of a panel. Use it to disable clipping for the rendering
	// of this panel.
	virtual void DisableClipping( bool bDisable ) = 0;

	virtual void SetClippingRect( int left, int top, int right, int bottom ) = 0;

	// Sets the mouse get + set callbacks
	virtual void SetMouseCallbacks( GetMouseCallback_t getFunc, SetMouseCallback_t setFunc ) = 0;

	// Installs a function to play sounds
	virtual void InstallPlaySoundFunc( PlaySoundFunc_t soundFunc ) = 0;

	// Some drawing methods that cannot be accomplished under Win32
	virtual void DrawColoredCircle( int centerx, int centery, float radius, int r, int g, int b, int a ) = 0;
	virtual void DrawColoredText( vgui::HFont font, int x, int y, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char *fmt, ... ) = 0;

	// Draws text with current font at position and wordwrapped to the rect using color values specified
	virtual void DrawColoredTextRect( vgui::HFont font, int x, int y, int w, int h, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char *fmt, ... ) = 0;
	virtual void DrawTextHeight( vgui::HFont font, int w, int& h, PRINTF_FORMAT_STRING char *fmt, ... ) = 0;

	// Returns the length of the text string in pixels
	virtual int	DrawTextLen( vgui::HFont font, PRINTF_FORMAT_STRING const char *fmt, ... ) = 0;

	// Draws a panel in 3D space. Assumes view + projection are already set up
	// Also assumes the (x,y) coordinates of the panels are defined in 640xN coords
	// (N isn't necessary 480 because the panel may not be 4x3)
	// The width + height specified are the size of the panel in world coordinates
	virtual void DrawPanelIn3DSpace( vgui::VPANEL pRootPanel, const VMatrix &panelCenterToWorld, int nPixelWidth, int nPixelHeight, float flWorldWidth, float flWorldHeight ) = 0; 

	// Binds a material to a surface texture ID
	virtual void DrawSetTextureMaterial( int id, IMaterial *pMaterial ) = 0;

	// Handles an input event, returns true if the event should be filtered from the rest of the game
	virtual bool HandleInputEvent( const InputEvent_t &event ) = 0;

	virtual void Set3DPaintTempRenderTarget( const char *pRenderTargetName ) = 0;
	virtual void Reset3DPaintTempRenderTarget( void ) = 0;

	// Gets a material bound to a surface texture ID
	virtual IMaterial *DrawGetTextureMaterial( int id ) = 0;
};


#endif // IMATSYSTEMSURFACE_H