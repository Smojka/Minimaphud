 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseActionsManagerComponentClass: GameComponentClass
 {
 }

 class BaseActionsManagerComponent: GameComponent
 {
     proto external UserActionContext GetContext(string contextName);
     proto external int GetActionsList(out notnull array<BaseUserAction> outActions);
     proto external int GetActionsCount();
     proto external int GetContextList(out notnull array<UserActionContext> outContexts);
     proto external int GetContextCount();
     proto external BaseUserAction GetFirstAction();
     proto external BaseUserAction FindAction(int actionID);
     proto external bool IsEnabled();

     // callbacks

     event protected string NicifyGeneratedContextName(string name) { return name; };
 }
