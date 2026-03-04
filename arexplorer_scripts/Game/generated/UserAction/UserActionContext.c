 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class UserActionContext: ScriptAndConfig
 {
     proto external owned string GetContextName();
     proto external owned string GetName();
     proto external int GetActionsList(out notnull array<BaseUserAction> outActions);
     proto external int GetActionsCount();
     proto external bool GetTransformationModel(out vector outMat[4]);
     proto external bool GetTransformationWorld(out vector outMat[4]);
     proto external bool IsOmnidirectional();
     proto external float GetRadius();
     proto external float GetHeight();
     proto external bool IsCapsule();
     proto external vector GetOrigin();
     proto external bool IsInVisibilityAngle(vector position);
     proto external UIInfo GetUIInfo();
 }
