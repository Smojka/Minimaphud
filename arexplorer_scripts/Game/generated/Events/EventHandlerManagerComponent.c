 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class EventHandlerManagerComponentClass: GameComponentClass
 {
 }

 class EventHandlerManagerComponent: GameComponent
 {
     proto void RaiseEvent(string eventName, int argsCount, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL);
     proto void RegisterScriptHandler(string eventName, Managed inst, func callback, bool delayed = true, bool singleUse = false);
     proto void RemoveScriptHandler(string eventName, Managed inst, func callback, bool delayed = true);
     proto external int GetEventHandlers(out notnull array<BaseEventHandler> outEventHandlers);
 }
