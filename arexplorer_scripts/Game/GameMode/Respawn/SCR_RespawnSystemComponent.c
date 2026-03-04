 class SCR_RespawnSystemComponentClass : RespawnSystemComponentClass
 {
 }

 [ComponentEditorProps(icon: HYBRID_COMPONENT_ICON)]
 class SCR_RespawnSystemComponent : RespawnSystemComponent
 {
     [Attribute(category: "Respawn System")]
     protected ref SCR_SpawnLogic m_SpawnLogic;

     [Attribute("1", uiwidget: UIWidgets.CheckBox, category: "Respawn System")]
     protected bool m_bEnableRespawn;

     [Attribute("1.5", desc: "Delay (in seconds) for opening deploy menu after death.")]
     protected float m_fDeployMenuOpenDelay;

     [Attribute("{A39BE59EB6F41125}Configs/Respawn/SpawnPointRequestResultInfoConfig.conf", desc: "Holds a config of all reasons why a specific spawnpoint can be disabled")]
     protected ResourceName m_sSpawnPointRequestResultInfoHolder;

     protected ref SCR_SpawnPointRequestResultInfoConfig m_SpawnPointRequestResultInfoHolder;

     // Instance of this component
     private static SCR_RespawnSystemComponent s_Instance = null;

     // The parent of this entity which should be a gamemode
     protected SCR_BaseGameMode m_pGameMode;
     // Parent entity's rpl component
     protected RplComponent m_pRplComponent;

     protected RplComponent m_RplComponent;

     // Preload
     protected ref SimplePreload m_Preload;

     protected ref ScriptInvoker Event_OnRespawnEnabledChanged;

     //------------------------------------------------------------------------------------------------
  SCR_BaseSpawnPointRequestResultInfo GetSpawnPointRequestResultInfo(SCR_SpawnRequestComponent requestComponent, SCR_ESpawnResult response, SCR_SpawnData data)
     {
         if (!m_SpawnPointRequestResultInfoHolder)
             return null;

         return m_SpawnPointRequestResultInfoHolder.GetFirstValidRequestResultInfo(requestComponent, response, data);
     }

     //------------------------------------------------------------------------------------------------
  static SCR_RespawnSystemComponent GetInstance()
     {
         if (!s_Instance)
         {
             BaseGameMode pGameMode = GetGame().GetGameMode();
             if (pGameMode)
                 s_Instance = SCR_RespawnSystemComponent.Cast(pGameMode.FindComponent(SCR_RespawnSystemComponent));
         }

         return s_Instance;
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_FactionManager.SGetLocalPlayerFaction instead")]
     static Faction GetLocalPlayerFaction(IEntity player = null)
     {
         SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(player);
         if (character)
             return character.GetFaction();

         return SCR_FactionManager.SGetLocalPlayerFaction();
     }

     //------------------------------------------------------------------------------------------------
     RplComponent GetRplComponent()
     {
         return m_RplComponent;
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_SpawnHandlerComponent instead.")]
     protected override GenericEntity RequestSpawn(int playerId);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_RespawnComponent.RequestSpawn instead!")]
     GenericEntity CustomRespawn(int playerId, string prefab, vector position, vector rotation = vector.Zero);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Utilize SCR_LoadoutManager instead!")]
     bool CanSetLoadout(int playerId, int loadoutIndex);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_PlayerLoadoutComponent instead!")]
     void DoSetPlayerLoadout(int playerId, int loadoutIndex);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_PlayerFactionAffiliationComponent instead")]
     bool CanSetFaction(int playerId, int factionIndex);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Spawn points are no longer assigned and are utilized directly through SCR_SpawnPointSpawnHandlerComponent")]
     bool CanSetSpawnPoint(int playerId, RplId spawnPointId);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Spawn points are no longer assigned and are utilized directly through SCR_SpawnPointSpawnHandlerComponent")]
     void DoSetPlayerSpawnPoint(int playerId, RplId spawnPointIdentity);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_FactionManager.SGetFactionPlayerCount instead.")]
     int GetFactionPlayerCount(Faction faction)
     {
         return SCR_FactionManager.SGetFactionPlayerCount(faction);
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_LoadoutManager.SGetLoadoutPlayerCount instead")]
     int GetLoadoutPlayerCount(SCR_BasePlayerLoadout loadout)
     {
         return SCR_LoadoutManager.SGetLoadoutPlayerCount(loadout);
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_PlayerLoadoutComponent.RequestLoadout instead")]
     void SetPlayerLoadout(int playerId, int loadoutIndex);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_PlayerFactionAffiliationComponent instead.")]
     void SetPlayerFaction(int playerId, int factionIndex);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_FactionManager instead.")]
     Faction GetFactionByIndex(int factionIndex);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_FactionManager instead.")]
     int GetFactionIndex(Faction faction)
     {
         return -1;
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_FactionManager.SGetPlayerFaction instead")]
     Faction GetPlayerFaction(int playerId)
     {
         return SCR_FactionManager.SGetPlayerFaction(playerId);
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_LoadoutManager instead!")]
     SCR_BasePlayerLoadout GetLoadoutByIndex(int loadoutIndex);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_LoadoutManager.SGetPlayerLoadout instead")]
     SCR_BasePlayerLoadout GetPlayerLoadout(int playerId);

     //------------------------------------------------------------------------------------------------
     [Obsolete("Use SCR_LoadoutManager instead!")]
     int GetLoadoutIndex(SCR_BasePlayerLoadout loadout)
     {
         return -1;
     }

     //------------------------------------------------------------------------------------------------
  static MenuBase OpenRespawnMenu()
     {
         MenuManager pMenuManager = GetGame().GetMenuManager();
         if (!pMenuManager)
             return null;

         return pMenuManager.OpenMenu(ChimeraMenuPreset.RespawnSuperMenu);
     }

     //------------------------------------------------------------------------------------------------
  static void CloseRespawnMenu()
     {
         MenuManager pMenuManager = GetGame().GetMenuManager();
         if (!pMenuManager)
             return;

         MenuBase menu = pMenuManager.FindMenuByPreset(ChimeraMenuPreset.RespawnSuperMenu);
         if (menu)
             pMenuManager.CloseMenu(menu);
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("...")]
     static bool IsRespawnMenuOpened()
     {
         MenuManager pMenuManager = GetGame().GetMenuManager();
         if (!pMenuManager)
             return false;

         return (pMenuManager.FindMenuByPreset(ChimeraMenuPreset.RespawnSuperMenu) != null);
     }

     //------------------------------------------------------------------------------------------------
     [Obsolete("...")]
     static void ToggleRespawnMenu()
     {
         if (IsRespawnMenuOpened())
             CloseRespawnMenu();
         else
             OpenRespawnMenu();
     }

     //------------------------------------------------------------------------------------------------
  void ServerSetEnableRespawn(bool enableSpawning)
     {
         if (enableSpawning == m_bEnableRespawn || !Replication.IsServer())
             return;

         SetEnableRespawnBroadcast(enableSpawning);
         Rpc(SetEnableRespawnBroadcast, enableSpawning);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
     protected void SetEnableRespawnBroadcast(bool enableSpawning)
     {
         m_bEnableRespawn = enableSpawning;
         if (Event_OnRespawnEnabledChanged)
             Event_OnRespawnEnabledChanged.Invoke(m_bEnableRespawn);
     }

     //------------------------------------------------------------------------------------------------
  bool IsRespawnEnabled()
     {
         return m_bEnableRespawn;
     }

     //------------------------------------------------------------------------------------------------
  bool IsFactionChangeAllowed()
     {
         return m_pGameMode.IsFactionChangeAllowed();
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnRespawnEnabledChanged()
     {
         if (!Event_OnRespawnEnabledChanged)
             Event_OnRespawnEnabledChanged = new ScriptInvoker();

         return Event_OnRespawnEnabledChanged;
     }

     //------------------------------------------------------------------------------------------------
  bool CanRequestSpawn_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, SCR_SpawnData data, out SCR_ESpawnResult result = SCR_ESpawnResult.SPAWN_NOT_ALLOWED)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::CanRequestSpawn_S(playerId: %2, handler: %2, data: %3)", Type().ToString(),
                     requestComponent.GetPlayerId(),
                     handlerComponent,
                     data), LogLevel.NORMAL);
         #endif

         if (!m_bEnableRespawn)
         {
             result = SCR_ESpawnResult.NOT_ALLOWED_SPAWNING_DISABLED;
             return false;
         }

         return m_pGameMode.CanPlayerSpawn_S(requestComponent, handlerComponent, data, result);
     }

     //------------------------------------------------------------------------------------------------
  bool PreparePlayerEntity_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, SCR_SpawnData data, IEntity entity)
     {
         return m_pGameMode.PreparePlayerEntity_S(requestComponent, handlerComponent, data, entity);
     }

     //------------------------------------------------------------------------------------------------
  void OnPlayerEntityChange_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, IEntity previousEntity, IEntity newEntity, SCR_SpawnData data)
     {
         m_pGameMode.OnPlayerEntityChanged_S(requestComponent.GetPlayerId(), previousEntity, newEntity);
         m_SpawnLogic.OnPlayerEntityChanged_S(requestComponent.GetPlayerId(), previousEntity, newEntity);
     }

     //------------------------------------------------------------------------------------------------
  void OnSpawnPlayerEntityFailure_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, IEntity entity, SCR_SpawnData data, SCR_ESpawnResult reason)
     {
         m_pGameMode.OnSpawnPlayerEntityFailure_S(requestComponent, handlerComponent, entity, data, reason);
     }

     //------------------------------------------------------------------------------------------------
  void OnPlayerSpawnFinalize_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnHandlerComponent handlerComponent, SCR_SpawnData data, IEntity entity)
     {
         m_pGameMode.OnPlayerSpawnFinalize_S(requestComponent, handlerComponent, data, entity);
         m_SpawnLogic.OnPlayerSpawned_S(requestComponent.GetPlayerId(), entity);
     }

     //------------------------------------------------------------------------------------------------
  void OnPlayerRegistered_S(int playerId)
     {
         m_SpawnLogic.OnPlayerRegistered_S(playerId);
     }

     //------------------------------------------------------------------------------------------------
  void OnPlayerDisconnected_S(int playerId, KickCauseCode cause, int timeout)
     {
         m_SpawnLogic.OnPlayerDisconnected_S(playerId, cause, timeout);
     }

     //------------------------------------------------------------------------------------------------
  void OnPlayerKilled_S(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer)
     {
         m_SpawnLogic.OnPlayerKilled_S(playerId, playerEntity, killerEntity, killer);
     }

     //------------------------------------------------------------------------------------------------
  void OnPlayerDeleted_S(int playerId)
     {
         m_SpawnLogic.OnPlayerDeleted_S(playerId);
     }

     //------------------------------------------------------------------------------------------------
  float GetDeployMenuOpenDelay_ms()
     {
         return m_fDeployMenuOpenDelay * 1000;
     }

     //------------------------------------------------------------------------------------------------
  bool CanOpenDeployMenu()
     {
         return m_SpawnLogic && m_SpawnLogic.Type() == SCR_MenuSpawnLogic;
     }

     //------------------------------------------------------------------------------------------------
  SCR_SpawnLogic GetSpawnLogic()
     {
         return m_SpawnLogic;
     }

     //------------------------------------------------------------------------------------------------
     override void OnInit(IEntity owner)
     {
         m_SpawnPointRequestResultInfoHolder = SCR_ConfigHelperT<SCR_SpawnPointRequestResultInfoConfig>.GetConfigObject(m_sSpawnPointRequestResultInfoHolder);
         if (!m_SpawnPointRequestResultInfoHolder)
             Print("'SCR_RespawnSystemComponent' has no valid m_SpawnPointRequestResultInfoHolder! This means the disabled reason cannot be disabled spawn point!", LogLevel.ERROR);

         m_pGameMode = SCR_BaseGameMode.Cast(owner);
         if (!m_pGameMode)
             Print("SCR_RespawnSystemComponent has to be attached to a SCR_BaseGameMode (or inherited) entity!", LogLevel.ERROR);
         m_RplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));

         if (!m_SpawnLogic)
             Print("SCR_RespawnSystemComponent is missing SCR_SpawnLogic!", LogLevel.ERROR);

         if (GetGame().InPlayMode())
         {
             m_SpawnLogic.OnInit(this);

             // Validate faction manager
             SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
             if (!factionManager)
             {
                 string text = string.Format("No %1 found in the world, %2 might not work as intended!",
                         SCR_FactionManager, SCR_RespawnSystemComponent);
                 Print(text, LogLevel.WARNING);
             }

             // Validate loadout manager
             SCR_LoadoutManager loadoutManager = GetGame().GetLoadoutManager();
             if (!loadoutManager)
             {
                 string text = string.Format("No %1 found in the world, %2 might not work as intended!",
                         SCR_LoadoutManager, SCR_RespawnSystemComponent);
                 Print(text, LogLevel.WARNING);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
     protected override event bool OnRplSave(ScriptBitWriter w)
     {
         w.WriteBool(m_bEnableRespawn);

         return super.OnRplSave(w);
     }

     //------------------------------------------------------------------------------------------------
     protected override event bool OnRplLoad(ScriptBitReader r)
     {
         bool enableRespawn;
         r.ReadBool(enableRespawn);

         SetEnableRespawnBroadcast(enableRespawn);

         return super.OnRplLoad(r);
     }

     //------------------------------------------------------------------------------------------------
     // destructor
     void ~SCR_RespawnSystemComponent()
     {
         s_Instance = null;
     }
 }