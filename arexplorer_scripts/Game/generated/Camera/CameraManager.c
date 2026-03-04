 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CameraManagerClass: GenericEntityClass
 {
 }

 class CameraManager: GenericEntity
 {
     proto external CameraBase CurrentCamera();
     proto external bool SetCamera(CameraBase pCam);
     proto external void SetNextCamera();
     proto external void SetPreviousCamera();
     proto external int GetCamerasList(out notnull array<CameraBase> outCameras);
     proto external void SetOverlayCamera(CameraBase pCam);
     proto external CameraBase GetOverlayCamera();
     proto external void SetFirstPersonFOV(float fieldOfView);
     proto external float GetFirstPersonFOV();
     proto external void SetThirdPersonFOV(float fieldOfView);
     proto external float GetThirdPersonFOV();
     proto external void SetVehicleFOV(float fieldOfView);
     proto external float GetVehicleFOV();
     proto external float GetClampedFOV(float fieldOfView);
     proto external float GetMinFOV();
     proto external float GetMaxFOV();

     // callbacks

     event protected void OnInit();
 }
