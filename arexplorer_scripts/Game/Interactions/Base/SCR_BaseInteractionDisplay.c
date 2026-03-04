 //------------------------------------------------------------------------------------------------
 enum ActionFinishReason
 {
     FINISHED,
     INTERRUPTED

     // FINISHED: Action was sucessfuly finished, i.e. duration was fulfilled
     // INTERRUPTED: Action was interrupted, i.e. user stopped performing the action midway
 };

 //------------------------------------------------------------------------------------------------
 class ActionsTuple
 {
     ref array<BaseUserAction> param1;
     ref array<bool> param2;

     void Init()
     {
         param1 = new array<BaseUserAction>();
         param2 = new array<bool>();
     }
 };

 //------------------------------------------------------------------------------------------------
 class ActionDisplayData
 {
     IEntity pUser;
     ref ActionsTuple pActionsData;
     BaseUserAction pSelectedAction;
     UserActionContext pCurrentContext;
 };

 //------------------------------------------------------------------------------------------------
 class SCR_BaseInteractionDisplay : SCR_InfoDisplayExtended
 {
     void OnActionStart( IEntity pUser, BaseUserAction pPerformedAction );

     void OnActionProgress( IEntity pUser, BaseUserAction pPerformedAction, float fProgress, float fDuration );

     void OnActionFinish( IEntity pUser, BaseUserAction pFinishedAction, ActionFinishReason eFinishReason );

     void ShowDisplay();

     void HideDisplay();

     void SetDisplayData(ActionDisplayData data);
 };