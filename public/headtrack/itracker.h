#pragma once

class Camera_Intrinsics;
class CCameraCapture;
class Cam_IMU_Calibration;
class VMatrix;
class TWorldFiducial;

class ITracker
{
public:
	virtual bool BPreInit() = 0;
	virtual bool BInit() = 0;
	virtual void BPostInit() = 0;
	virtual void Shutdown() = 0;
	virtual const char* GetDisplayName() = 0;
	virtual void GetWindowBounds(int* windowWidth, int* windowHeight, int* pnX, int* pnY, int* renderWidth, int* renderHeight) = 0;
	virtual void BGetCurrentCameraFromWorldPose(VMatrix* pResultCameraFromWorldPose, VMatrix* pResultCameraFromWorldPoseUnpredicted = NULL, double* pflAcquireTime = NULL) = 0;
	virtual void GetWorldFiducials(TWorldFiducial*, unsigned int) = 0;
	virtual bool ShouldUseNeckModel() = 0;
	virtual bool MayDriftInYaw() = 0;
	virtual int GetValidFrameSequenceNumber() = 0;
	virtual float GetUIAspectRatio() = 0;
	virtual void SetCameraCapture(Camera_Intrinsics& intrinsics, CCameraCapture* capture) = 0;
	virtual void SetCamIMUCalibration(const Cam_IMU_Calibration& calibration) = 0;
	virtual const char* GetHmdCalibrationFileName(bool param) = 0;
	virtual const char* GetSerialNumber() = 0;
	virtual const char* GetModelNumber() = 0;
};