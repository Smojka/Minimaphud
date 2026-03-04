 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseScriptedCameraItem
 {
     event void OnActivate(ScriptedCameraItem pPrevCamera, ScriptedCameraItemResult pPrevCameraResult);
     event void OnDeactivate(ScriptedCameraItem pNextCamera);
     event void OnBlendIn();
     event void OnBlendOut();
     event void OnUpdate(float pDt, out ScriptedCameraItemResult pOutResult);
     event void SetBaseAngles(out vector angles);
 }
