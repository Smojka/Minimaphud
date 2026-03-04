 [ComponentEditorProps(category: "GameScripted/Camera", description: "")]
 class SCR_BaseCameraComponentClass : ScriptComponentClass
 {
 }

 class SCR_BaseCameraComponent : ScriptComponent
 {
     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_BaseCameraComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
     {
         if (SCR_CameraBase.Cast(ent) == null)
         {
             Print(string.Format("Component %1 must be attached to SCR_CameraBase!", Type()), LogLevel.ERROR);
             Deactivate(ent);
         }
     }
 }