 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ScriptedGameTriggerEntityClass: BaseGameTriggerEntityClass
 {
 }

 class ScriptedGameTriggerEntity: BaseGameTriggerEntity
 {
     event protected event void OnInit(IEntity owner);
     event protected event void OnFrame(IEntity owner, float timeSlice);
     event bool ScriptedEntityFilterForQuery(IEntity ent) { return true; };
 }
