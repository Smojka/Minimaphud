 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseScriptedCameraSet
 {
     proto external void RegisterCameraCreator(int cameraId, typename pCameraType);

     // callbacks

     event void Init();
     event protected float OnCameraChange(int pFrom, int pTo) { return 0.3; };
 }
