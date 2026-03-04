 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class GameStatsApi
 {
     proto external void PlayerEvent(int iPlayerID, Managed params);
     proto external void PlayerScore(int iPlayerID, Managed params);
     proto external void EditorStart();
     proto external void EditorClosed();
     proto external void IncrementEditorCounter(int iPlayerID);
     proto external void ModEvent(Managed params);
 }
