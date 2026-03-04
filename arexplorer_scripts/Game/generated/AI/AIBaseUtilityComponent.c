 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class AIBaseUtilityComponentClass: AIComponentClass
 {
 }

 class AIBaseUtilityComponent: AIComponent
 {
     proto external AIActionBase EvaluateActions();
     proto external void AddAction(AIActionBase action);
     proto external bool RemoveObsoleteActions();
     proto external void SetStateAllActionsOfType(typename actionType, EAIActionState actionState, bool includeInherited = false);
     proto external bool HasActionOfType(typename actionType);
     proto external AIActionBase FindActionOfType(typename actionType);
     proto external AIActionBase FindActionOfInheritedType(typename actionType);
     proto external void FindActionsOfType(typename actionType, notnull array<ref AIActionBase> outActions);
     proto external void FindActionsOfInheritedType(typename actionType, notnull array<ref AIActionBase> outActions);
     proto external void SetStateOfRelatedAction(AIActionBase relatedAction, EAIActionState state);
     proto external bool CallActionsOnMessage(AIMessage msg);
     //Getters and Setters
     proto external void GetActions(notnull array<ref AIActionBase> outActions);
     proto external ref AIActionBase GetCurrentAction();
     proto external void SetCurrentAction(AIActionBase executed);
     proto external ref AIActionBase GetExecutedAction();
     proto external void SetExecutedAction(AIActionBase executed);
     proto external AIAgent GetOwner();

     // callbacks

     event void EOnInit(IEntity owner);
     event void OnPostInit(IEntity owner);
     event void EOnDeactivate(IEntity owner);
     event void EOnDiag(IEntity owner, float timeSlice);
     event void EOnActivate(IEntity owner);
 }
