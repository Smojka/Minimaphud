 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class MissionHeader: ScriptAndConfig
 {
     proto external string GetWorldPath();
     proto external string GetHeaderResourcePath();
     proto ResourceName GetHeaderResourceName();
     static proto ref MissionHeader ReadMissionHeader(string path);
 }
