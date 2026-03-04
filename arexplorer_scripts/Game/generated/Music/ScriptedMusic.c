 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ScriptedMusic: Music
 {
     proto external string GetName();
     proto external void SetName(string name);

     // callbacks

     event bool ShouldPlay();
     event void Init();
     event void Update(float dt);
     event void OnDelete();
     event void OnPlay();
     event void OnStop();
 }
