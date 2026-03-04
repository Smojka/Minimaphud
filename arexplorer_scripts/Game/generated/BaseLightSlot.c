 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseLightSlot: ScriptAndConfig
 {
     proto external bool ShouldTick();
     proto external int GetLightSide();
     // returns id of Parent emissive surface. -1 in case parent surface is not set
     proto external int GetSurfaceID();
     proto external bool IsPresence();
     proto external bool IsLightFunctional();
     proto external IEntity GetOwner();
     proto external IEntity GetLightEntity();
     proto external ELightType GetLightType();
     proto external int GetLightID();
     proto external void SetLightFunctional(bool state);

     // callbacks

     event protected void OnInit(IEntity owner);
     event protected void OnLightStateChanged(bool newState);
 }