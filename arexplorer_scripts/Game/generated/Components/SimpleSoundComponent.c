 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class SimpleSoundComponentClass: BaseSoundComponentClass
 {
 }

 class SimpleSoundComponent: BaseSoundComponent
 {
     proto external void EnableDynamicSimulation(bool value);
     proto external void SetScriptedMethodsCall(bool state);
     proto external bool IsScriptedMethodsCallEnabled();
     proto external IEntity GetOwner();

     // callbacks

     event protected void OnPostInit(IEntity owner);
     event protected void OnInit(IEntity owner);
     event protected void OnFrame(IEntity owner, float timeSlice);
     event protected void UpdateSoundJob(IEntity owner, float timeSlice);
     event protected void OnUpdateSoundJobBegin(IEntity owner);
     event protected void OnUpdateSoundJobEnd(IEntity owner);
 }
