 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseItemAnimationComponentClass: AnimationControllerComponentClass
 {
 }

 class BaseItemAnimationComponent: AnimationControllerComponent
 {
     proto external IEntity GetOwner();
     proto external bool SyncWithCharacter(ChimeraCharacter pCharacter, string overrideStartNode);
     proto external bool RemoveSyncReference(ChimeraCharacter pCharacter);
     proto external bool IsAnimationTag(AnimationTagID tagID);

     // callbacks

     event protected void OnAnimationEvent(AnimationEventID animEventType, AnimationEventID animUserString, int intParam, float timeFromStart, float timeToEnd);
     event protected bool OnPrepareAnimInput(IEntity owner, float ts) { return false; };
     event protected bool OnProcessAnimOutput(IEntity owner, float ts) { return false; };
     event protected void OnCharacterFloatVariablet(int variableID, float value);
     event protected void OnCharacterIntVariable(int variableID, int value);
     event protected void OnCharacterBoolVariable(int variableID, bool value);
     event protected void OnCharacterCommand(int commandID, int intValue, float floatValue);
 }
