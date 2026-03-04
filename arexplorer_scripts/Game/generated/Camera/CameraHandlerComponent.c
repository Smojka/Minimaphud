 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CameraHandlerComponentClass: ScriptComponentClass
 {
 }

 class CameraHandlerComponent: ScriptComponent
 {
     proto external ScriptedCameraSet GetCameraSet();
     proto external bool IsInThirdPerson();
     proto external void SetThirdPerson(bool val);
     proto external float GetFocusMode();
     proto external void SetFocusMode(float focusMode);
     proto external float GetBlendAlpha(ScriptedCameraItem forCameraItem);
     proto external ScriptedCameraItem GetCurrentCamera();
     proto external bool IsCameraBlending();
     proto external void SetLensFlareSet(CameraLensFlareSetType type, string lensFlareSetUserName);

     // callbacks

     event protected float CalculateFovScalar(notnull CameraBase mainCamera, CameraBase overlayCamera) { return 1.0; };
     event void Init();
     event void OnCameraActivate();
     event void OnCameraDeactivate();
     event int CameraSelector();
     event void OnBeforeCameraUpdate(float pDt, bool pIsKeyframe);
     event void OnAfterCameraUpdate(float pDt, bool pIsKeyframe, inout vector transformMS[4], inout vector transformWS[4]);
     event void CollisionSolver(float pDt, inout ScriptedCameraItemResult pOutResult, inout vector resCamTM[4], bool isKeyframe);
     event void OnThirdPersonSwitch(bool isInThirdPerson);
     event float GetCameraTransitionTime(int pFrom, int pTo);
     event float GetOverlayCameraFOVScalarWeight();
 }
