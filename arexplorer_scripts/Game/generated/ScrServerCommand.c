 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ScrServerCommand
 {
     event string GetKeyword();
     event protected ref ScrServerCmdResult OnUpdate();
     event protected ref ScrServerCmdResult OnRCONExecution(array<string> argv);
     event protected int RequiredRCONPermission();
     event bool IsServerSide();
     event protected ref ScrServerCmdResult OnChatServerExecution(array<string> argv, int playerId);
     event protected ref ScrServerCmdResult OnChatClientExecution(array<string> argv, int playerId);
     event protected int RequiredChatPermission();
 }