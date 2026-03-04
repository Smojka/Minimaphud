 class SCR_SpawnHandlerComponentClass : ScriptComponentClass
 {
 }

 class SCR_SpawnHandlerComponent : ScriptComponent
 {
     private SCR_RespawnSystemComponent m_RespawnSystemComponent;

     //------------------------------------------------------------------------------------------------
  protected SCR_RespawnSystemComponent GetRespawnSystemComponent()
     {
         return m_RespawnSystemComponent;
     }

     [Attribute(category: "Respawn Handler", desc: "If enabled, previous controlled entity prior to respawn will be destroyed (killed).")]
     protected bool m_bDestroyPreviousControlledEntity;

     [Attribute(category: "Respawn Handler", desc: "If enabled, previous controlled entity prior to respawn will be destroyed (killed).")]
     protected bool m_bDeletePreviousControlledEntity;

     //------------------------------------------------------------------------------------------------
  protected override void OnPostInit(IEntity owner)
     {
         super.OnPostInit(owner);

         m_RespawnSystemComponent = SCR_RespawnSystemComponent.Cast(owner.FindComponent(SCR_RespawnSystemComponent));
         if (!m_RespawnSystemComponent)
             Debug.Error(string.Format("%1 could not find %2! (should be a child of)", Type().ToString(), SCR_RespawnSystemComponent));
     }

     //------------------------------------------------------------------------------------------------
  SCR_ESpawnResult CanHandleRequest_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data)
     {
         int playerId = requestComponent.GetPlayerController().GetPlayerId();
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::CanHandleRequest_S(playerId: %2, data: %3)", Type().ToString(),
                     playerId,
                     data), LogLevel.NORMAL);
         #endif

         // The authority can activate and deactivate respawn handlers at will, but
         // only the active ones should resolve incoming requests. This should allow
         // the user to enable and disable individual respawning methods on the fly
         if (!IsActive())
             return SCR_ESpawnResult.UNSUPPORTED_SPAWN_METHOD;

         // Now that data is updated, validate the data prior to
         // doing any game specific checks. An invalid request
         // is dropped without ever reaching the game.
         if (!ValidateData_S(requestComponent, data))
             return SCR_ESpawnResult.CANNOT_VALIDATE;

         // See if player can be spawned with provided data,
         // this can be e.g. based on gamemodes
         SCR_ESpawnResult requestResult;
         if (!CanRequestSpawn_S(requestComponent, data, requestResult))
             return requestResult;

         return SCR_ESpawnResult.OK;
     }

     //------------------------------------------------------------------------------------------------
  SCR_ESpawnResult HandleRequest_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, out IEntity spawnedEntity)
     {
         int playerId = requestComponent.GetPlayerController().GetPlayerId();
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::HandleRequest_S(playerId: %2, data: %3)", Type().ToString(),
                     playerId,
                     data), LogLevel.NORMAL);
         #endif

         // Validate whether spawn can happen
         SCR_ESpawnResult canSpawn = CanHandleRequest_S(requestComponent, data);
         if (canSpawn != SCR_ESpawnResult.OK)
             return canSpawn;

         #ifdef ENABLE_DIAG
         if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_COMPONENT_TIME))
             Debug.BeginTimeMeasure();
         #endif

         // Process with spawn
         SCR_ESpawnResult result = SpawnEntity_S(requestComponent, data, spawnedEntity);
         if (result == SCR_ESpawnResult.OK && !spawnedEntity)
         {
             Debug.Error(string.Format("Bad %1 implementation! Result is %2, but entity is %3!",
                 Type().ToString(), typename.EnumToString(SCR_ESpawnResult, result), spawnedEntity));

             result = SCR_ESpawnResult.INTERNAL_ERROR;
         }

         #ifdef ENABLE_DIAG
         if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_COMPONENT_TIME))
             Debug.EndTimeMeasure("SpawnEntity_S");
         #endif

         return result;
     }

     //------------------------------------------------------------------------------------------------
  void OnFinalizeBegin_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, IEntity entity)
     {
         if (SCR_BaseGameMode.Cast(GetGame().GetGameMode()).CanStartSpawnPreload())
             requestComponent.StartSpawnPreload(data.GetPosition());
     }

     //------------------------------------------------------------------------------------------------
  bool CanFinalize_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, IEntity entity)
     {
         return !requestComponent.IsPreloading();
     }

     //------------------------------------------------------------------------------------------------
  SCR_ESpawnResult FinalizeRequest_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, IEntity entity)
     {
         int playerId = requestComponent.GetPlayerId();
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::FinalizeRequest_S(playerId: %2, data: %3)", Type().ToString(),
                     playerId,
                     data), LogLevel.NORMAL);
         #endif

         if (!AssignEntity_S(requestComponent, entity, data))
             return SCR_ESpawnResult.CANNOT_ASSIGN;

         SCR_RespawnSystemComponent respawnSystem = GetRespawnSystemComponent();
         respawnSystem.OnPlayerSpawnFinalize_S(requestComponent, this, data, entity);
         OnFinalizeDone_S(requestComponent, data, entity);
         return SCR_ESpawnResult.OK;
     }

     //------------------------------------------------------------------------------------------------
  void OnFinalizeDone_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, IEntity entity);

     //------------------------------------------------------------------------------------------------
  protected bool ValidateData_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::ValidateData_S(playerId: %1, data: %2)", Type().ToString(), requestComponent.GetPlayerId(), data), LogLevel.NORMAL);
         #endif
         return data.IsValid();
     }

     //------------------------------------------------------------------------------------------------
  bool CanRequestSpawn_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, out SCR_ESpawnResult result)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::CanRequestSpawn_S(playerId: %2, data: %3)", Type().ToString(),
                     requestComponent.GetPlayerId(), data), LogLevel.NORMAL);
         #endif

         SCR_RespawnSystemComponent respawnSystem = SCR_RespawnSystemComponent.GetInstance();
         return respawnSystem.CanRequestSpawn_S(requestComponent, this, data, result);
     }

     //------------------------------------------------------------------------------------------------
  protected SCR_ESpawnResult SpawnEntity_S(SCR_SpawnRequestComponent requestComponent, notnull SCR_SpawnData data, out IEntity spawnedEntity)
     {
         int playerId = requestComponent.GetPlayerId();
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::SpawnEntity_S(playerId: %2, data: %3)", Type().ToString(),
                     playerId,
                     data), LogLevel.NORMAL);
         #endif

         ResourceName prefab = data.GetPrefab();
         if (!ValidatePrefab_S(prefab))
             return SCR_ESpawnResult.INVALID_PREFAB;

         IEntity previousEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);

         // Transformation
         EntitySpawnParams params = new EntitySpawnParams();
         params.TransformMode = ETransformMode.WORLD;
         vector angles = data.GetAngles();
         vector yawPitchRoll = Vector(angles[1], angles[0], angles[2]);
         Math3D.AnglesToMatrix(yawPitchRoll, params.Transform);
         params.Transform[3] = data.GetPosition();

         #ifdef ENABLE_DIAG
         if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_COMPONENT_TIME))
             Debug.BeginTimeMeasure();
         #endif

         // Spawn
         Resource res = Resource.Load(prefab);

         #ifdef ENABLE_DIAG
         if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_COMPONENT_TIME))
         {
             Debug.EndTimeMeasure("Resource::Load");
             Debug.BeginTimeMeasure();
         }
         #endif

         IEntity entity = GetGame().SpawnEntityPrefab(res, params: params);

         #ifdef ENABLE_DIAG
         if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_COMPONENT_TIME))
             Debug.EndTimeMeasure("Game::SpawnEntityPrefab");
         #endif

         if (!PrepareEntity_S(requestComponent, entity, data))
         {
             HandleSpawnEntityFailure_S(requestComponent, entity, data, SCR_ESpawnResult.CANNOT_PREPARE);
             return SCR_ESpawnResult.CANNOT_PREPARE;
         }

         spawnedEntity = entity;
         return SCR_ESpawnResult.OK;
     }

     //------------------------------------------------------------------------------------------------
  protected bool ValidatePrefab_S(ResourceName resourceName)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::ValidatePrefab_S(pref: %2)", Type().ToString(), resourceName), LogLevel.NORMAL);
         #endif

         if (resourceName.IsEmpty())
             return false;

         Resource res = Resource.Load(resourceName);
         if (!res.IsValid())
             return false;

         IEntityComponentSource rplComponent = SCR_BaseContainerTools.FindComponentSource(res, RplComponent);
         if (!rplComponent)
             return false;

         IEntityComponentSource baseControllerComponent = SCR_BaseContainerTools.FindComponentSource(res, RplComponent);
         if (!baseControllerComponent)
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
  protected bool PrepareEntity_S(SCR_SpawnRequestComponent requestComponent, IEntity entity, SCR_SpawnData data)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::PrepareEntity_S(playerId: %2, entity: %3, data: %4)", Type().ToString(),
                     requestComponent.GetPlayerController().GetPlayerId(),
                     entity,
                     data), LogLevel.NORMAL);
         #endif

         return GetRespawnSystemComponent().PreparePlayerEntity_S(requestComponent, this, data, entity);
     }

     //------------------------------------------------------------------------------------------------
  protected bool AssignEntity_S(SCR_SpawnRequestComponent requestComponent, IEntity entity, SCR_SpawnData data)
     {
         int playerId = requestComponent.GetPlayerController().GetPlayerId();
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::AssignEntity_S(playerId: %2, entity: %3, data: %4)", Type().ToString(),
                     playerId,
                     entity,
                     data), LogLevel.NORMAL);
         #endif

         SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));

         // Already controlling!
         IEntity previous = playerController.GetControlledEntity();
         if (previous == entity)
             return false;

         playerController.SetInitialMainEntity(entity);

         // Notify the system of change
         HandleEntityChange_S(requestComponent, previous, entity, data);
         return true;
     }

     //------------------------------------------------------------------------------------------------
  protected void HandleSpawnEntityFailure_S(SCR_SpawnRequestComponent requestComponent, IEntity entity, SCR_SpawnData data, SCR_ESpawnResult reason)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::HandleSpawnEntityFailure_S(playerId: %2, entity: %2, data: %3, res: %4)", Type().ToString(),
                     requestComponent.GetPlayerId(), entity, data, typename.EnumToString(SCR_ESpawnResult, reason)), LogLevel.NORMAL);
         #endif

         SCR_RespawnSystemComponent respawnSystem = GetRespawnSystemComponent();
         respawnSystem.OnSpawnPlayerEntityFailure_S(requestComponent, this, entity, data, reason);

         if (entity && ShouldDeleteEntityOnSpawnFailure_S(requestComponent, entity, data, reason))
             RplComponent.DeleteRplEntity(entity, false);
     }

     //------------------------------------------------------------------------------------------------
  protected bool ShouldDeleteEntityOnSpawnFailure_S(SCR_SpawnRequestComponent requestComponent, IEntity entity, SCR_SpawnData data, SCR_ESpawnResult reason)
     {
         return true;
     }

     //------------------------------------------------------------------------------------------------
  protected void HandleEntityChange_S(SCR_SpawnRequestComponent requestComponent, IEntity previousEntity, IEntity newEntity, SCR_SpawnData data)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::HandleEntityChange_S(playerId: %2, previousEntity: %3, newEntity: %4, data: %5)", Type().ToString(),
                     requestComponent.GetPlayerController().GetPlayerId(),
                     previousEntity,
                     newEntity,
                     data), LogLevel.NORMAL);
         #endif

         // Notify manager
         SCR_RespawnSystemComponent respawnSystem = GetRespawnSystemComponent();
         respawnSystem.OnPlayerEntityChange_S(requestComponent, this, previousEntity, newEntity, data);

         // No need to resolve
         if (!previousEntity)
             return;

         if (m_bDestroyPreviousControlledEntity)
         {
             DamageManagerComponent damageManager = DamageManagerComponent.Cast(previousEntity.FindComponent(DamageManagerComponent));
             if (damageManager)
                 damageManager.SetHealthScaled(0.0);
         }

         if (m_bDeletePreviousControlledEntity)
             RplComponent.DeleteRplEntity(previousEntity, false);
     }
 }