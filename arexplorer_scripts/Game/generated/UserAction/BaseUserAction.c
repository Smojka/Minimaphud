 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseUserAction: ScriptAndConfig
 {
     // Injects
  protected string m_sCannotPerformReason;

     void SetCannotPerformReason(string reason)
     {
         m_sCannotPerformReason = reason;
     }

     string GetCannotPerformReason()
     {
         return m_sCannotPerformReason;
     }

     string ActionNameParams[9];

     proto external int GetContextIndex(string sContextName);
     proto external void SetActiveContext(UserActionContext context);
     proto external UserActionContext GetActiveContext();
     proto external bool CanBePerformed(IEntity user);
     proto external bool CanBeShown(IEntity user);
     proto external bool HasLocalEffectOnly();
     proto external bool CanBroadcast();
     proto external bool ShouldPerformPerFrame();
     proto external string GetActionName();
     proto external string GetActionDescription();
     proto external float GetVisibilityRange();
     proto external void SetActionDuration(float duration);
     proto external float GetActionDuration();
     proto external float GetActionProgress();
     proto external bool IsInProgress();
     proto external UIInfo GetUIInfo();
     proto external IEntity GetOwner();
     proto external bool CanAggregate();
     proto external int GetActionID();
     proto external void SetSendActionDataFlag();

     // callbacks

     event protected bool OnRplSave(ScriptBitWriter writer) { return true; };
     event protected bool OnRplLoad(ScriptBitReader reader) { return true; };
     event protected bool OnSaveActionData(ScriptBitWriter writer) { return true; };
     event protected bool OnLoadActionData(ScriptBitReader reader) { return true; };
     event protected event void OnActionSelected();
     event protected event void OnActionDeselected();
 }
