 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class ChimeraGame: Game
 {
     proto external bool IsExperimentalBuild();
     proto external ScriptWorldClock GetClock();
     proto external GameSignalsManager GetSignalsManager();
     proto external PlayerController GetPlayerController();
     proto external PlayerManager GetPlayerManager();
     // returns the Perception Manager
     proto external PerceptionManager GetPerceptionManager();
     proto external FactionManager GetFactionManager();
     proto external BaseChatEntity GetChat();
     proto external CameraManager GetCameraManager();
     proto external BaseGameMode GetGameMode();
     proto external MapEntity GetMapManager();
     proto external void SetViewDistance(float viewDistance);
     proto external float GetViewDistance();
     proto external float GetMaximumViewDistance();
     proto external float GetMinimumViewDistance();
     proto external void SetGrassDistance(int grassDistance);
     proto external int GetGrassDistance();
     proto external int GetMaximumGrassDistance();
     proto external int GetMinimumGrassDistance();
     proto external float GetViewDistanceServerLimit();
     proto external bool IsVONUIDisabledByServer();
     proto external bool IsVONDirectSpeechUIDisabledByServer();
     proto external bool GetVONCanTransmitCrossFaction();
     proto external MissionHeader GetMissionHeader();
     proto external AIWorld GetAIWorld();
     proto external GameStatsApi GetStatsApi();
     proto external void LogFPS(int msec);
     static proto bool CanSpawnEntityPrefab(notnull Resource templateResource, EntitySpawnParams params = null);
     proto external IEntity SpawnEntityPrefabLocal(notnull Resource templateResource, BaseWorld world = null, EntitySpawnParams params = null);

     // callbacks

     event bool GetIsClientAuthority();
     event void OnCinematicStart();
     event void OnCinematicEnd();
     event void OnCinematicBlending(float blendFactor, vector cameraPosition);
     event protected void OnMissionSet(MissionHeader mission);
     event protected void OnKickedFromGame(KickCauseCode kickCode);
     event protected void ShowErrorMessage(string msg);
     event protected ref Managed GetPlayerDataStats(int playerID);
     event protected string GetMissionName();
 }