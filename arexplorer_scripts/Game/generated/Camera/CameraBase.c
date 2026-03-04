 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class CameraBaseClass: GenericEntityClass
 {
 }

 class CameraBase: GenericEntity
 {
     proto external void ApplyTransform(float timeslice);
     proto external void GetWorldCameraTransform(out vector mat[4]);
     proto external IEntity GetCursorTarget();
     proto external IEntity GetCursorTargetWithPosition(out vector outPosition);
     proto external CameraType GetProjType();
     proto external float GetVerticalFOV();
     proto external float GetNearPlane();
     proto external float GetFarPlane();
     proto external void SetFOVDegree(float fovDegrees);
     proto external void SetVerticalFOV(float fov);
     proto external void SetNearPlane(float nearPlane);
     proto external void SetFarPlane(float farPlane);
     proto external void SetCameraIndex(int cameraIndex);
     proto external int GetCameraIndex();
     proto external void SetLensFlareSet(CameraLensFlareSetType type, string userName);
     proto external bool IsAABBVisible(vector mins, vector maxs);
     proto external bool IsSphereVisible(vector origin, float radius);

     // callbacks

     event protected void CameraActivate();
     event protected void CameraDeactivate();
 }
