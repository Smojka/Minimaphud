 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseLightManagerComponentClass: GameComponentClass
 {
 }

 class BaseLightManagerComponent: GameComponent
 {
     proto external bool GetLightsEnabled();
     proto external void SetLightsState(ELightType type, bool state = false, int side = -1);
     proto external bool GetLightsState(ELightType type, int side = -1);
     proto external BaseLightSlot FindLight(int lightID);
     proto external int GetLights(out notnull array<BaseLightSlot> outLights);
     proto external bool TrySetSurfaceFunctional(int surfaceID, bool functiona);

     // callbacks

     event protected void OnInit(IEntity owner);
     event protected void OnFrame(IEntity owner, float timeSlice);
 }
