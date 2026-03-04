 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ExtBaseInteractionHandlerComponentClass: BaseInteractionHandlerComponentClass
 {
 }

 class ExtBaseInteractionHandlerComponent: BaseInteractionHandlerComponent
 {
     proto external protected void SetSelectedAction(BaseUserAction action);
     proto external UserActionContext GetCurrentContext();
     proto external IEntity GetControlledEntity();
     proto external int GetFilteredActions(out notnull array<BaseUserAction> outActions, out notnull array<bool> outCanBePerformed);
     proto external bool IsContextAvailable();
     proto external void SetManualCollectionOverride(bool enabled);
     proto external bool GetManualCollectionOverride();
     proto external void SetNearbyCollectionEnabled(bool enabled);
     proto external bool GetNearbyCollectionEnabled();
     proto external float GetNearbyCollectionRadius();
     proto external int GetNearbyAvailableContextList(out notnull array<UserActionContext> outContexts);
     proto external int GetNearbyShowableContextList(out notnull array<UserActionContext> outContexts);
     proto external int GetNearbyUnavailableContextList(out notnull array<UserActionContext> outContexts);

     // callbacks

     event protected void OnContextChanged(UserActionContext previousContext, UserActionContext newContext);
     event protected void OnPostFrame(IEntity owner, IEntity controlledEntity, float timeSlice);
     event protected bool CanContextChange(UserActionContext currentContext, UserActionContext newContext);
     event protected bool DoIntersectCheck(IEntity controlledEntity);
     event protected bool GetIsInteractionAvailableScript();
     event protected bool GetCanInteractScript(IEntity controlledEntity);
     event protected BaseUserAction GetSelectedActionScript();
     event protected array<IEntity> GetManualOverrideList(IEntity owner, out vector referencePoint);
 }
