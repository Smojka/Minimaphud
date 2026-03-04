 [ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Base for gamemode scripted component.")]
 class SCR_BaseGameModeComponentClass : ScriptComponentClass
 {
 }

 class SCR_BaseGameModeComponent : ScriptComponent
 {
     protected SCR_BaseGameMode m_pGameMode;

     //------------------------------------------------------------------------------------------------
  SCR_BaseGameMode GetGameMode()
     {
         return m_pGameMode;
     }

     //------------------------------------------------------------------------------------------------
  void OnGameEnd();

     //------------------------------------------------------------------------------------------------
  void OnGameStateChanged(SCR_EGameModeState state);

     //------------------------------------------------------------------------------------------------
  void OnGameModeStart();

     //------------------------------------------------------------------------------------------------
  void OnGameModeEnd(SCR_GameModeEndData data);

     //------------------------------------------------------------------------------------------------
  void OnPlayerConnected(int playerId);

     //------------------------------------------------------------------------------------------------
  void OnPlayerAuditSuccess(int playerId);

     //------------------------------------------------------------------------------------------------
  void OnPlayerAuditFail(int playerId);

     //------------------------------------------------------------------------------------------------
  void OnPlayerAuditTimeouted(int playerId);

     //------------------------------------------------------------------------------------------------
  void OnPlayerAuditRevived(int playerId);

     //------------------------------------------------------------------------------------------------
  void OnPlayerRegistered(int playerId);

     //------------------------------------------------------------------------------------------------
  void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use OnPlayerSpawnFinalize_S instead")]
     void OnPlayerSpawned(int playerId, IEntity controlledEntity);

     //------------------------------------------------------------------------------------------------
  bool PreparePlayerEntity_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, SCR_SpawnData data, IEntity entity)
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  void OnSpawnPlayerEntityFailure_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, IEntity entity, SCR_SpawnData data, SCR_ESpawnResult reason);

     //------------------------------------------------------------------------------------------------
  void OnPlayerSpawnFinalize_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, SCR_SpawnData data, IEntity entity);

     //------------------------------------------------------------------------------------------------
  bool HandlePlayerKilled(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer)
     {
         return true; // by default, handle automatically
     }

     //------------------------------------------------------------------------------------------------
  void OnPlayerKilled(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer);

     //------------------------------------------------------------------------------------------------
  void OnPlayerKilledHandled(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer);

     //------------------------------------------------------------------------------------------------
  void OnPlayerDeleted(int playerId, IEntity player);

     //------------------------------------------------------------------------------------------------
  void OnPlayerRoleChange(int playerId, EPlayerRole roleFlags);

     //------------------------------------------------------------------------------------------------
  void OnWorldPostProcess(World world);

     //------------------------------------------------------------------------------------------------
     [Obsolete()]
     void HandleOnLoadoutAssigned(int playerID, SCR_BasePlayerLoadout assignedLoadout);

     //------------------------------------------------------------------------------------------------
     [Obsolete()]
     void HandleOnFactionAssigned(int playerID, Faction assignedFaction);

     //------------------------------------------------------------------------------------------------
     [Obsolete()]
     void HandleOnSpawnPointAssigned(int playerID, SCR_SpawnPoint spawnPoint);

     //------------------------------------------------------------------------------------------------
  void OnControllableSpawned(IEntity entity);

     //------------------------------------------------------------------------------------------------
  void OnControllableDestroyed(IEntity entity, IEntity killerEntity, notnull Instigator killer);

     //------------------------------------------------------------------------------------------------
  void OnControllableDeleted(IEntity entity);

     //------------------------------------------------------------------------------------------------
     // constructor
  void SCR_BaseGameModeComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
     {
         m_pGameMode = SCR_BaseGameMode.Cast(ent);
         if (!m_pGameMode)
         {
             string message = string.Format("%1 is attached to entity '%2' type=%3, required type=%4! This is not allowed!", Type().ToString(), ent.GetName(), ent.ClassName(), "SCR_BaseGameMode");
             Debug.Error(message);
             Print(message, LogLevel.WARNING);
         }
     }
 }