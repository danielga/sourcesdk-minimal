#pragma once
#include <mathlib/vmatrix.h>
#include <keyvalues.h>
#include <appframework/iappsystem.h>

struct TEyeCalibration
{
	struct TEyeEdges
	{
		int iTop, iBot, iIn, iOut;		// These are used to calculate everything else.
		float fSizePixels;
		float fReliefInches;
	} Left, Right;

	float fIpdPixels;
	float fIpdInches;
};

enum HeadtrackMovementMode_t
{
	HMM_SHOOTFACE_MOVEFACE = 0,		// Shoot from your face, move along your face.
	HMM_SHOOTFACE_MOVETORSO,		// Shoot from your face, move the direction your torso is facing.
	HMM_SHOOTMOUSE_MOVEFACE,		// Shoot from the mouse cursor which moves within the HUD, move along your face.
	HMM_SHOOTBOUNDEDMOUSE_LOOKFACE_MOVEFACE,	// Shoot from the mouse cursor which moves, bounded within the HUD, move along your face.
	HMM_SHOOTBOUNDEDMOUSE_LOOKFACE_MOVEMOUSE,	// Shoot from the mouse cursor which moves, bounded within the HUD, move along your weapon (the "mouse")

	// The following are not intended to be user-selectable modes, they are used by e.g. followcam stuff.
	HMM_SHOOTMOVELOOKMOUSEFACE,		// Shoot & move & look along the mouse cursor (i.e. original unchanged gameplay), face just looks on top of that.
	HMM_SHOOTMOVEMOUSE_LOOKFACE,	// Shoot & move along the mouse cursor (i.e. original unchanged gameplay), face just looks.
	HMM_SHOOTMOVELOOKMOUSE,			// Shoot, move and look along the mouse cursor - HMD orientation is completely ignored!

	HMM_LAST,

	HMM_NOOVERRIDE = HMM_LAST,		// Used as a retrun from ShouldOverrideHeadtrackControl(), not an actual mode.
	HMM_SHOOTTRACK  // Shoot per tracked offset & move view relative
};

enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10, // Draws a quad allowing stencil test to clear through portals
	VIEW_CLEAR_STENCIL = 0x20,
};

enum StereoEye_t
{
	STEREO_EYE_MONO = 0,
	STEREO_EYE_LEFT = 1,
	STEREO_EYE_RIGHT = 2,
	STEREO_EYE_MAX = 3,
};


//-----------------------------------------------------------------------------
// Purpose: Renderer setup data.  
//-----------------------------------------------------------------------------
class CViewSetup
{
public:
	CViewSetup()
	{
		m_flAspectRatio = 0.0f;
		m_bRenderToSubrectOfLargerScreen = false;
		m_bDoBloomAndToneMapping = true;
		m_bOrtho = false;
		m_bOffCenter = false;
		m_bCacheFullSceneState = false;
//		m_bUseExplicitViewVector = false;
		m_bViewToProjectionOverride = false;
		m_eStereoEye = STEREO_EYE_MONO;
	}

// shared by 2D & 3D views

	// left side of view window
	int			x;					
	int			m_nUnscaledX;
	// top side of view window
	int			y;					
	int			m_nUnscaledY;
	// width of view window
	int			width;				
	int			m_nUnscaledWidth;
	// height of view window
	int			height;				
	// which eye are we rendering?
	StereoEye_t m_eStereoEye;
	int			m_nUnscaledHeight;

// the rest are only used by 3D views

	// Orthographic projection?
	bool		m_bOrtho;			
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;		
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// horizontal FOV in degrees
	float		fov;				
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;		

	// 3D origin of camera
	Vector		origin;					

	// heading of camera (pitch, yaw, roll)
	QAngle		angles;				
	// local Z coordinate of near plane of camera
	float		zNear;			
		// local Z coordinate of far plane of camera
	float		zFar;			

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;		
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;		

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Controls for off-center projection (needed for poster rendering)
	bool		m_bOffCenter;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;

	// Control that the SFM needs to tell the engine not to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState;

	// If using VR, the headset calibration will feed you a projection matrix per-eye.
	// This does NOT override the Z range - that will be set up as normal (i.e. the values in this matrix will be ignored).
	bool		m_bViewToProjectionOverride;
	VMatrix	 m_ViewToProjection;
};

#define HEADTRACK_INTERFACE_VERSION "VHeadTrack001"

class THeadTrackResults;
class TWorldFiducial;
struct THeadTrackParms;
class IHeadTrack : public IAppSystem
{
public:
	virtual bool Connect(CreateInterfaceFn fn) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char*) = 0;
	virtual InitReturnVal_t Init() = 0;
	virtual void Shutdown() = 0;

	virtual ~IHeadTrack() {};
	virtual const char* GetDisplayName() = 0;
	virtual void GetWindowBounds(int* windowWidth, int* windowHeight, int* pnX, int* pnY, int* renderWidth, int* renderHeight) = 0;
	virtual IHeadTrack* CreateInstance() = 0;
	virtual void ResetTracking() = 0;
	virtual void SetCurrentCameraAsZero() = 0;
	virtual void GetCameraFromWorldPose(VMatrix* pResultCameraFromWorldPose, VMatrix* pResultCameraFromWorldPoseUnpredicted = NULL, double* pflAcquireTime = NULL) = 0;
	virtual void GetCameraPoseZeroFromCurrent(VMatrix* pResultMatrix) = 0;
	virtual void GetCurrentEyeTransforms(THeadTrackResults&, THeadTrackParms&) = 0;
	virtual void GetWorldFiducials(TWorldFiducial*, uint) = 0;
	virtual void ProcessCurrentTrackingState(float PlayerGameFov) = 0;
	virtual void OverrideView(CViewSetup* pViewMiddle, Vector* pViewModelOrigin, QAngle* pViewModelAngles, HeadtrackMovementMode_t hmmMovementOverride) = 0;
	virtual void OverrideStereoView(CViewSetup* pViewMiddle, CViewSetup* pViewLeft, CViewSetup* pViewRight) = 0;
	virtual void OverridePlayerMotion(float flInputSampleFrametime, const QAngle& oldAngles, const QAngle& curAngles, const Vector& curMotion, QAngle* pNewAngles, Vector* pNewMotion) = 0;
	virtual void OverrideWeaponHudAimVectors(Vector* pAimOrigin, Vector* pAimDirection) = 0;
	virtual void OverrideZNearFar(float*, float*) = 0;
	virtual void OverrideTorsoTransform(const Vector& position, const QAngle& angles) = 0;
	virtual void CancelTorsoTransformOverride() = 0;
	virtual void GetTorsoRelativeAim(Vector* pPosition, QAngle* pAngles) = 0;
	virtual const VMatrix& GetWorldFromMidEye() = 0;
	virtual float GetZoomedModeMagnification() = 0;
	virtual void GetCurrentEyeViewport(int&, int&, int&, int&) = 0;
	virtual void SetCurrentStereoEye(StereoEye_t) = 0;
	virtual bool DoDistortionProcessing(const vrect_t* srcRect) = 0;
	virtual void AlignTorsoAndViewToWeapon() = 0;
	//virtual void OverrideViewModelTransform(Vector& vmorigin, QAngle& vmangles, bool bUseLargeOverride) = 0; // NOTE: Some function here doesn't exist on windows... idk which one it is but removing this one fixes the vtable offsets for now.
	virtual bool ShouldRenderHUDInWorld() = 0;
	virtual float GetHUDDistance() = 0;
	virtual bool ShouldRenderStereoHUD() = 0;
	virtual void RefreshCameraTexture() = 0;
	virtual bool IsCameraTextureAvailable() = 0;
	virtual void RenderHUDQuad(bool bBlackout, bool bTranslucent) = 0;
	virtual const VMatrix& GetHudProjectionFromWorld() = 0;
	virtual bool CollectSessionStartStats(KeyValues* pkvStats) = 0;
	virtual bool CollectPeriodicStats(KeyValues* pkvStats) = 0;
	virtual void RecalcEyeCalibration(TEyeCalibration* p) = 0;
	virtual void GetCurrentEyeCalibration(TEyeCalibration* p) = 0;
	virtual void SetCurrentEyeCalibration(TEyeCalibration const& p) = 0;
	virtual void SetEyeCalibrationDisplayMisc(int iEditingNum, bool bVisible) = 0;
};