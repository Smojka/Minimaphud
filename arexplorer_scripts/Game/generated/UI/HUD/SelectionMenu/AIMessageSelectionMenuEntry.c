 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class AIMessageSelectionMenuEntry: ScriptedSelectionMenuEntry
 {
     void AIMessageSelectionMenuEntry(AIMessage pAIMessage)
     {
         this.SetAIMessage(pAIMessage);
     }

     proto external void SetAIMessage(AIMessage pAIMessage);
     proto external AIMessage GetAIMessage();
 }
