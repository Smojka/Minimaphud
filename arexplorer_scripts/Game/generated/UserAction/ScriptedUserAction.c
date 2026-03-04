 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ScriptedUserAction: BaseUserAction
 {
     event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent);
     event void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity);
     event void PerformContinuousAction(IEntity pOwnerEntity, IEntity pUserEntity, float timeSlice);
     event void OnConfirmed(IEntity pUserEntity);
     event void OnRejected(IEntity pUserEntity);
     event void OnActionCanceled(IEntity pOwnerEntity, IEntity pUserEntity) { };
     event void OnActionStart(IEntity pUserEntity) { };
     event bool GetActionNameScript(out string outName) { return false; };
     event bool GetActionDescriptionScript(out string outName) { return false; };
     event bool CanBePerformedScript(IEntity user) { return true; };
     event bool CanBeShownScript(IEntity user) { return true; };
     event bool HasLocalEffectOnlyScript() { return false; };
     event bool CanBroadcastScript() { return true; };
 }
