 [ComponentEditorProps(category: "GameScripted/Network", description: "")]
 class SCR_NotificationsComponentClass: ScriptComponentClass
 {
     [Attribute("{7134157CA6B1FA8E}Configs/Notifications/Notifications.conf", desc: "Link to config that holds all notification data", params: "conf class=SCR_NotificationConfig")]
     protected ResourceName m_sNotificationConfig;

     protected ref map<ENotification, ref SCR_NotificationDisplayData> m_mNotificationDisplayDataMap = new map<ENotification, ref SCR_NotificationDisplayData>();

     protected ref SCR_NotificationConfig notificationConfig;

     //------------------------------------------------------------------------------------------------
  SCR_NotificationDisplayData GetNotificationDisplayData(ENotification notificationID)
     {
         SCR_NotificationDisplayData notificationData;

         if (m_mNotificationDisplayDataMap.Find(notificationID, notificationData))
             return notificationData;

         Print("Notification data not found  in 'SCR_NotificationsComponent' for key: '" + typename.EnumToString(ENotification, notificationID) + "'.", LogLevel.WARNING);
         m_mNotificationDisplayDataMap.Find(ENotification.UNKNOWN, notificationData);

         return notificationData;
     }

     //------------------------------------------------------------------------------------------------
     array<string> GetStickyNotifications()
     {
         return notificationConfig.GetStickyNotifications();
     }

     //------------------------------------------------------------------------------------------------
     array<ref SCR_NotificationDisplayColor> GetNotificationDisplayColor()
     {
         return notificationConfig.GetNotificationDisplayColor();
     }

     //------------------------------------------------------------------------------------------------
     // constructor
  //~ Constructor creates notification map for the system to get the notification data for each ENotification
     void SCR_NotificationsComponentClass(BaseContainer prefab)
     {
         //~ Notification map already filled
         if (!m_mNotificationDisplayDataMap.IsEmpty())
             return;

         //~ Get config
         notificationConfig = SCR_NotificationConfig.Cast(SCR_BaseContainerTools.CreateInstanceFromPrefab(m_sNotificationConfig, true));
         if (!notificationConfig)
         {
             Print("'SCR_NotificationsComponentClass' failed to load notifications config!", LogLevel.ERROR);
             return;
         }

         //~ Get data
         array<ref SCR_NotificationDisplayData> data = {};
         int count = notificationConfig.GetNotificationData(data);

         //~ Gegenerate
         for(int i = 0; i < count; i++)
         {
             if (!m_mNotificationDisplayDataMap.Contains(data[i].m_NotificationKey))
                 m_mNotificationDisplayDataMap.Set(data[i].m_NotificationKey, data[i]);
             else
                 Print("Notification data in 'SCR_NotificationsLogDisplay' has duplicate notification info key: '" + typename.EnumToString(ENotification, data[i].m_NotificationKey) + "'. There should only be one of each key!", LogLevel.WARNING);
         }
     }
 }

 class SCR_NotificationsComponent : ScriptComponent
 {
     protected ref array<ref SCR_NotificationData> m_aHistory = {};
     protected ref ScriptInvoker Event_OnNotification = new ScriptInvoker();

     protected SCR_NotificationData m_LastNotificationWithLocation;
     protected bool m_bIsUpdatingNotificationData;

     //How long each notification exists before it gets deleted, Time in seconds
     static const int NOTIFICATION_DELETE_TIME = 30;

     //How many notifications are remembered in history.
     static const int NOTIFICATION_HISTORY_LENGTH = 10;

     //Hot fix for when player names cannot be found
     protected ref map <int, string> m_mPlayerNameHistory = new map <int, string>();

     //------------------------------------------------------------------------------------------------
     ScriptInvoker GetOnNotification()
     {
         return Event_OnNotification;
     }

     //------------------------------------------------------------------------------------------------
  int GetHistoryNewToOld(out notnull array<SCR_NotificationData> outHistory)
     {
         int count = m_aHistory.Count();
         foreach (SCR_NotificationData entry : m_aHistory)
         {
             outHistory.Insert(entry);
         }
         return count;
     }

     //------------------------------------------------------------------------------------------------
  int GetHistoryOldToNew(out notnull array<SCR_NotificationData> outHistory, int maxHistoryIndex = -1)
     {
         int count = m_aHistory.Count();
         if (maxHistoryIndex > 0 && count > maxHistoryIndex)
             count = maxHistoryIndex;

         for (int i = count -1; i >= 0; i--)
         {
            outHistory.Insert(m_aHistory[i]);
         }
         return count;
     }

     //------------------------------------------------------------------------------------------------
  bool GetHistoryEntry(int index, out SCR_NotificationData data = null)
     {
         if (index < m_aHistory.Count())
         {
             data = m_aHistory[index];
             return true;
         }
         else
         {
             return false;
         }
     }

     //------------------------------------------------------------------------------------------------
  bool GetLastNotificationLocation(out vector lastLocation)
     {
         if (!m_LastNotificationWithLocation || !m_LastNotificationWithLocation.GetDisplayData())
             return false;

         return m_LastNotificationWithLocation.GetDisplayData().GetPosition(m_LastNotificationWithLocation, lastLocation);
     }

     //------------------------------------------------------------------------------------------------
  static SCR_NotificationsComponent GetInstance()
     {
         PlayerController playerController = GetGame().GetPlayerController();
         if (playerController)
             return SCR_NotificationsComponent.Cast(playerController.FindComponent(SCR_NotificationsComponent));
         else
             return null;
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToEveryone(ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendToEveryone(notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToEveryone(ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         //~ Send from client to server to broadcast
         if (!Replication.IsServer())
         {
             SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(SCR_PlayerController.GetLocalPlayerId()));
             if (!playerController)
                 return false;

             SCR_NotificationsComponent notificationsComponent = SCR_NotificationsComponent.Cast(playerController.FindComponent(SCR_NotificationsComponent));
             if (!notificationsComponent)
                 return false;

             array<int> paramArray = {};
             CreateParamArray(paramArray, param1, param2, param3, param4, param5, param6);

             notificationsComponent.Rpc(notificationsComponent.Rpc_SendToEveryone, notificationID, position, paramArray);
             return true;
         }

         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.EVERYONE, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendToEveryoneData(notificationID, newNotificationData);
     }

     //------------------------------------------------------------------------------------------------
     //~ Allows client to broadcast notification to everyone
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void Rpc_SendToEveryone(ENotification notificationID, vector position, notnull array<int> paramArray)
     {
         int param1, param2, param3, param4, param5, param6;
         GetParamsFromArray(paramArray, param1, param2, param3, param4, param5, param6);

         SendToEveryone(notificationID, position, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
     //Set the actual data to players
     protected static bool SendToEveryoneData(ENotification notificationID, SCR_NotificationData data)
     {
         //--- Only server can broadcast messages
         if (!Replication.IsServer())
             return false;

         array<int> players = {};
         for (int i = 0, count = GetGame().GetPlayerManager().GetPlayers(players); i < count; i++)
         {
             SendToPlayerData(players[i], notificationID, data);
         }
         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToPlayer(int playerID, ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendToPlayer(playerID, notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToPlayer(int playerID, ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         //~ Send from client to server
         if (!Replication.IsServer())
         {
             SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(SCR_PlayerController.GetLocalPlayerId()));
             if (!playerController)
                 return false;

             SCR_NotificationsComponent notificationsComponent = SCR_NotificationsComponent.Cast(playerController.FindComponent(SCR_NotificationsComponent));
             if (!notificationsComponent)
                 return false;

             array<int> paramArray = {};
             CreateParamArray(paramArray, param1, param2, param3, param4, param5, param6);

             notificationsComponent.Rpc(notificationsComponent.Rpc_SendToPlayer, playerID, notificationID, position, paramArray);
             return true;
         }

         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.SPECIFIC_PLAYER, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendToPlayerData(playerID, notificationID, newNotificationData);
     }

     //------------------------------------------------------------------------------------------------
     //~ Allows client to broadcast notification to specific player
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void Rpc_SendToPlayer(int playerID, ENotification notificationID, vector position, notnull array<int> paramArray)
     {
         int param1, param2, param3, param4, param5, param6;
         GetParamsFromArray(paramArray, param1, param2, param3, param4, param5, param6);

         SendToPlayer(playerID, notificationID, position, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
     //Set the actual data to player
     protected static bool SendToPlayerData(int playerID, ENotification notificationID, SCR_NotificationData data)
     {
         //~ Only server can broadcast messages
         if (!Replication.IsServer())
             return false;

         PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerID);
         if (!playerController)
             return false;

         SCR_NotificationsComponent notificationsComponent = SCR_NotificationsComponent.Cast(playerController.FindComponent(SCR_NotificationsComponent));
         if (!notificationsComponent)
             return false;

         notificationsComponent.SendToOwner(notificationID, data);
         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToFaction(notnull SCR_Faction faction, bool includeFriendlyFactions, ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendToFaction(faction, includeFriendlyFactions, notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToFaction(notnull SCR_Faction faction, bool includeFriendlyFactions, ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         //~ Send from client to server
         if (!Replication.IsServer())
         {
             SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(SCR_PlayerController.GetLocalPlayerId()));
             if (!playerController)
                 return false;

             SCR_NotificationsComponent notificationsComponent = SCR_NotificationsComponent.Cast(playerController.FindComponent(SCR_NotificationsComponent));
             if (!notificationsComponent)
                 return false;

             FactionManager factionManager = GetGame().GetFactionManager();
             if (!factionManager)
                 return false;

             array<int> paramArray = {};
             CreateParamArray(paramArray, param1, param2, param3, param4, param5, param6);

             notificationsComponent.Rpc(notificationsComponent.Rpc_SendToFaction, factionManager.GetFactionIndex(faction), includeFriendlyFactions, notificationID, position, paramArray);
             return true;
         }

         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.SPECIFIC_PLAYER, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendToFactionData(faction, includeFriendlyFactions, notificationID, newNotificationData);
     }

     //------------------------------------------------------------------------------------------------
     //~ Allows client to broadcast notification to faction
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void Rpc_SendToFaction(int factionIndex, bool includeFriendlyFactions, ENotification notificationID, vector position, notnull array<int> paramArray)
     {
         FactionManager factionManager = GetGame().GetFactionManager();
         if (!factionManager)
             return;

         SCR_Faction faction = SCR_Faction.Cast(factionManager.GetFactionByIndex(factionIndex));
         if (!faction)
             return;

         int param1, param2, param3, param4, param5, param6;
         GetParamsFromArray(paramArray, param1, param2, param3, param4, param5, param6);

         SendToFaction(faction, includeFriendlyFactions, notificationID, position, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
     //Set the actual data to players in faction
     protected static bool SendToFactionData(notnull SCR_Faction faction, bool includeFriendlyFactions, ENotification notificationID, SCR_NotificationData data)
     {
         //--- Only server can broadcast messages
         if (!Replication.IsServer())
             return false;

         array<int> players = {};
         faction.GetPlayersInFaction(players);

         foreach(int playerID : players)
         {
             SendToPlayerData(playerID, notificationID, data);
         }

         if (!includeFriendlyFactions)
             return true;

         //~ If also include friendly factions send it to those factions as well
         FactionManager factionManager = GetGame().GetFactionManager();

         if (factionManager)
         {
             array<Faction> allFactions = {};
             SCR_Faction scrFaction;
             factionManager.GetFactionsList(allFactions);

             foreach(Faction otherFaction : allFactions)
             {
                 if (otherFaction == faction)
                     continue;

                 scrFaction = SCR_Faction.Cast(otherFaction);
                 if (!scrFaction)
                     continue;

                 if (!faction.IsFactionFriendly(scrFaction))
                     continue;

                 //~ Send to all players
                 scrFaction.GetPlayersInFaction(players);
                 foreach(int playerID : players)
                 {
                     SendToPlayerData(playerID, notificationID, data);
                 }
             }
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToUnlimitedEditorPlayers(ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendToUnlimitedEditorPlayers(notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToUnlimitedEditorPlayers(ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         //~ Send from client to server to broadcast
         if (!Replication.IsServer())
         {
             SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(SCR_PlayerController.GetLocalPlayerId()));
             if (!playerController)
                 return false;

             SCR_NotificationsComponent notificationsComponent = SCR_NotificationsComponent.Cast(playerController.FindComponent(SCR_NotificationsComponent));
             if (!notificationsComponent)
                 return false;

             array<int> paramArray = {};
             CreateParamArray(paramArray, param1, param2, param3, param4, param5, param6);

             notificationsComponent.Rpc(notificationsComponent.Rpc_SendToUnlimitedEditorPlayers, notificationID, position, paramArray);
             return true;
         }

         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.GM_ONLY, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendToUnlimitedEditorPlayersData(notificationID, newNotificationData);
     }

     //------------------------------------------------------------------------------------------------
     //~ Allows client to broadcast notification to players with unlimited editors
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void Rpc_SendToUnlimitedEditorPlayers(ENotification notificationID, vector position, notnull array<int> paramArray)
     {
         int param1, param2, param3, param4, param5, param6;
         GetParamsFromArray(paramArray, param1, param2, param3, param4, param5, param6);

         SendToUnlimitedEditorPlayers(notificationID, position, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
     //Set the actual data to players that have an unlimited editor
     protected static bool SendToUnlimitedEditorPlayersData(ENotification notificationID, SCR_NotificationData data, int playerID = -1)
     {
         //--- Only server can broadcast messages
         if (!Replication.IsServer())
             return false;

         array<int> players = {};
         for (int i = 0, count = GetGame().GetPlayerManager().GetPlayers(players); i < count; i++)
         {
             if (playerID == players[i])
             {
                 SendToPlayerData(players[i], notificationID, data);
                 continue;
             }

             SCR_EditorManagerCore core = SCR_EditorManagerCore.Cast(SCR_EditorManagerCore.GetInstance(SCR_EditorManagerCore));
             if (!core)
                 continue;

             SCR_EditorManagerEntity editorManager = core.GetEditorManager(players[i]);
             if (!editorManager)
                 continue;

             if (!editorManager.IsLimited())
                 SendToPlayerData(players[i], notificationID, data);
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToUnlimitedEditorPlayersAndPlayer(int playerID, ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendToUnlimitedEditorPlayersAndPlayer(playerID, notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToUnlimitedEditorPlayersAndPlayer(int playerID, ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         //~ Send from client to server to broadcast
         if (!Replication.IsServer())
         {
             SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(SCR_PlayerController.GetLocalPlayerId()));
             if (!playerController)
                 return false;

             SCR_NotificationsComponent notificationsComponent = SCR_NotificationsComponent.Cast(playerController.FindComponent(SCR_NotificationsComponent));
             if (!notificationsComponent)
                 return false;

             array<int> paramArray = {};
             CreateParamArray(paramArray, param1, param2, param3, param4, param5, param6);

             notificationsComponent.Rpc(notificationsComponent.Rpc_SendToUnlimitedEditorPlayersAndPlayer, playerID, notificationID, position, paramArray);
             return true;
         }

         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.GM_OR_AFFECTED_PLAYER_ONLY, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendToUnlimitedEditorPlayersData(notificationID, newNotificationData, playerID);
     }

     //------------------------------------------------------------------------------------------------
     //~ Allows client to broadcast notification to all players with unlimited editors and an the given specific player
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void Rpc_SendToUnlimitedEditorPlayersAndPlayer(int playerID, ENotification notificationID, vector position, notnull array<int> paramArray)
     {
         int param1, param2, param3, param4, param5, param6;
         GetParamsFromArray(paramArray, param1, param2, param3, param4, param5, param6);

         SendToUnlimitedEditorPlayersAndPlayer(playerID, notificationID, position, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToGroup(int groupID, ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendToGroup(groupID, notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendToGroup(int groupID, ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         //~ Send from client to server to broadcast
         if (!Replication.IsServer())
         {
             SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(SCR_PlayerController.GetLocalPlayerId()));
             if (!playerController)
                 return false;

             SCR_NotificationsComponent notificationsComponent = SCR_NotificationsComponent.Cast(playerController.FindComponent(SCR_NotificationsComponent));
             if (!notificationsComponent)
                 return false;

             array<int> paramArray = {};
             CreateParamArray(paramArray, param1, param2, param3, param4, param5, param6);

             notificationsComponent.Rpc(notificationsComponent.Rpc_SendToGroup, groupID, notificationID, position, paramArray);
             return true;
         }

         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.PLAYER_GROUP, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendToGroupData(notificationID, newNotificationData, groupID);
     }

     //------------------------------------------------------------------------------------------------
     //~ Allows client to broadcast notification to all players within the given group
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void Rpc_SendToGroup(int groupID, ENotification notificationID, vector position, notnull array<int> paramArray)
     {
         int param1, param2, param3, param4, param5, param6;
         GetParamsFromArray(paramArray, param1, param2, param3, param4, param5, param6);

         SendToGroup(groupID, notificationID, position, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
     //Set the actual data to players in the given group
     protected static bool SendToGroupData(ENotification notificationID, SCR_NotificationData data, int groupID)
     {
         //--- Only server can broadcast messages
         if (!Replication.IsServer())
             return false;

         SCR_GroupsManagerComponent groupManager = SCR_GroupsManagerComponent.GetInstance();
         if (!groupManager)
             return false;

         SCR_AIGroup group = groupManager.FindGroup(groupID);
         if (!group)
             return false;

         array<int> players = group.GetPlayerIDs();

         if (players.IsEmpty())
             return false;

         foreach (int playerID: players)
         {
             SendToPlayerData(playerID, notificationID, data);
         }

         return true;
     }

     //------------------------------------------------------------------------------------------------
  static bool SendLocal(ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendLocal(notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendLocal(ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.LOCAL_ONLY, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendLocalData(notificationID, newNotificationData);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendLocalUnlimitedEditor(ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendLocalUnlimitedEditor(notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendLocalUnlimitedEditor(ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         SCR_EditorManagerEntity editorManager = SCR_EditorManagerEntity.GetInstance();
         if (!editorManager || editorManager.IsLimited())
             return false;

         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.LOCAL_GM_ONLY, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendLocalData(notificationID, newNotificationData);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendLocalLimitedEditor(ENotification notificationID, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         return SendLocalLimitedEditor(notificationID, vector.Zero, param1, param2, param3, param4, param5, param6);
     }

     //------------------------------------------------------------------------------------------------
  static bool SendLocalLimitedEditor(ENotification notificationID, vector position, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         SCR_EditorManagerEntity editorManager = SCR_EditorManagerEntity.GetInstance();
         if (!editorManager || !editorManager.IsLimited())
             return false;

         SCR_NotificationData newNotificationData = SCR_NotificationData();
         newNotificationData.SetParameters(ENotificationReceiver.LOCAL_NON_GM_ONLY, param1, param2, param3, param4, param5, param6);
         newNotificationData.SetPosition(position);
         return SendLocalData(notificationID, newNotificationData);
     }

     //------------------------------------------------------------------------------------------------
     //Send actual notification data locally
     protected static bool SendLocalData(ENotification notificationID, SCR_NotificationData data)
     {
         SCR_NotificationsComponent notificationsComponent = GetInstance();
         if (!notificationsComponent)
             return false;

         notificationsComponent.ReceiveSCR_NotificationData(notificationID, data);//position, target);
         return true;
     }

     //------------------------------------------------------------------------------------------------
     //~ Create array of params
     protected static void CreateParamArray(notnull out array<int> paramArray, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         if (param1 != 0)
             paramArray.Insert(param1);

         if (param2 != 0)
             paramArray.Insert(param2);

         if (param3 != 0)
             paramArray.Insert(param3);

         if (param4 != 0)
             paramArray.Insert(param4);

         if (param5 != 0)
             paramArray.Insert(param5);

         if (param6 != 0)
             paramArray.Insert(param6);
     }

     //------------------------------------------------------------------------------------------------
     //~ Get all params from array of params
     protected static void GetParamsFromArray(notnull array<int> paramArray, out int param1 = 0, out int param2 = 0, out int param3 = 0, out int param4 = 0, out int param5 = 0, out int param6 = 0)
     {
         int count = paramArray.Count();

         if (count == 1)
         {
             param1 = paramArray[0];
         }
         else if (count == 2)
         {
             param1 = paramArray[0];
             param2 = paramArray[1];
         }
         else if (count == 3)
         {
             param1 = paramArray[0];
             param2 = paramArray[1];
             param3 = paramArray[2];
         }
         else if (count == 4)
         {
             param1 = paramArray[0];
             param2 = paramArray[1];
             param3 = paramArray[2];
             param4 = paramArray[3];
         }
         else if (count == 5)
         {
             param1 = paramArray[0];
             param2 = paramArray[1];
             param3 = paramArray[2];
             param4 = paramArray[3];
             param5 = paramArray[4];
         }
         else if (count == 6)
         {
             param1 = paramArray[0];
             param2 = paramArray[1];
             param3 = paramArray[2];
             param4 = paramArray[3];
             param5 = paramArray[4];
             param6 = paramArray[5];
         }
     }

     //------------------------------------------------------------------------------------------------
  bool SendToOwner(ENotification id, SCR_NotificationData data = null)
     {
         if (data)
             Rpc(ReceiveSCR_NotificationData, id, data);
         else
             Rpc(ReceiveNotification, id); //--- Don't send data when it's not needed (replication complains when null is received)

         return true;
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     protected void ReceiveNotification(ENotification id)
     {
         ReceiveSCR_NotificationData(id, null);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     protected void ReceiveSCR_NotificationData(ENotification id, SCR_NotificationData data)
     {
         SCR_NotificationData entry = data;
         if (!entry)
             entry = new SCR_NotificationData();

         //--- Set Meta data
         entry.SetMeta(id, GetNotificationDisplayData(id));

         //--- Save to history
         m_aHistory.InsertAt(entry, 0);
         m_aHistory.Resize(Math.Min(m_aHistory.Count(), NOTIFICATION_HISTORY_LENGTH));

         //--- Trigger event to be captured by other systems
         Event_OnNotification.Invoke(entry);
         //entry.Log();

         vector position;
         data.GetPosition(position);

         if (position != vector.Zero)
             m_LastNotificationWithLocation = data;

         //Start updating notification times if not yet done
         if (!m_bIsUpdatingNotificationData)
             UpdateNotificationData(true);
     }

     //------------------------------------------------------------------------------------------------
     array<string> GetStickyNotifications()
     {
         SCR_NotificationsComponentClass notificationClass = SCR_NotificationsComponentClass.Cast(GetComponentData(GetOwner()));
         if (!notificationClass)
             return null;

         return notificationClass.GetStickyNotifications();
     }

     //------------------------------------------------------------------------------------------------
     array<ref SCR_NotificationDisplayColor> GetNotificationDisplayColor()
     {
         SCR_NotificationsComponentClass notificationClass = SCR_NotificationsComponentClass.Cast(GetComponentData(GetOwner()));
         if (!notificationClass)
             return null;

         return notificationClass.GetNotificationDisplayColor();
     }

     //------------------------------------------------------------------------------------------------
     protected SCR_NotificationDisplayData GetNotificationDisplayData(ENotification notificationID)
     {
         SCR_NotificationsComponentClass notificationClass = SCR_NotificationsComponentClass.Cast(GetComponentData(GetOwner()));
         if (!notificationClass)
             return null;

         return notificationClass.GetNotificationDisplayData(notificationID);
     }

     //======================== PLAYER NAME HOTFIX ========================\\

     //------------------------------------------------------------------------------------------------
  string GetPlayerNameFromHistory(int playerID)
     {
         string playerName = string.Empty;
         m_mPlayerNameHistory.Find(playerID, playerName);

         return playerName;
     }

     //------------------------------------------------------------------------------------------------
     //~Hotfix to get player names when the player disconnected
     protected void AddPlayerNameToHistory(int playerID)
     {
         PlayerManager playerManager = GetGame().GetPlayerManager();
         if (!playerManager)
             return;

         m_mPlayerNameHistory.Set(playerID, playerManager.GetPlayerName(playerID));
     }

     //======================== START LISTENING TO UPDATE ========================\\

     //------------------------------------------------------------------------------------------------
     protected void UpdateNotificationData(bool updateNotificationData)
     {
         if (updateNotificationData == m_bIsUpdatingNotificationData)
             return;

         m_bIsUpdatingNotificationData = updateNotificationData;

         if (m_bIsUpdatingNotificationData)
             ConnectToNotificationsSystem();
         else
             DisconnectFromNotificationsSystem();
     }

     //------------------------------------------------------------------------------------------------
     protected void ConnectToNotificationsSystem()
     {
         World world = GetOwner().GetWorld();
         NotificationsSystem notificationsSystem = NotificationsSystem.Cast(world.FindSystem(NotificationsSystem));
         if (!notificationsSystem)
             return;

         notificationsSystem.Register(this);
     }

     //------------------------------------------------------------------------------------------------
     protected void DisconnectFromNotificationsSystem()
     {
         World world = GetOwner().GetWorld();
         NotificationsSystem notificationsSystem = NotificationsSystem.Cast(world.FindSystem(NotificationsSystem));
         if (!notificationsSystem)
             return;

         notificationsSystem.Unregister(this);
     }

     //------------------------------------------------------------------------------------------------
  void Update(float timeSlice)
     {
         int count = m_aHistory.Count();

         for(int i = 0; i < count; i++)
         {
            if (!m_aHistory[i])
                 continue;

             //Update the time left and check if it needs to be deleted
             if (m_aHistory[i].UpdateNotificationData(timeSlice))
             {
                 m_aHistory.RemoveOrdered(i);
                 i--;
                 count--;
             }
         }

         if (m_aHistory.IsEmpty())
             UpdateNotificationData(false);
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         PlayerController playerController = PlayerController.Cast(owner);
         if (!playerController)
             return;

         if (!m_aHistory.IsEmpty())
             UpdateNotificationData(true);

         //~Hotfix to remember player names even when the player left
         if (Replication.IsClient())
         {
             SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
             if (gameMode)
                 gameMode.GetOnPlayerRegistered().Insert(AddPlayerNameToHistory);

             //~ Get a list of connected players as OnPostInit might be called after connected players were registered
             array<int> players = {};

             GetGame().GetPlayerManager().GetPlayers(players);

             foreach (int player : players)
             {
                 AddPlayerNameToHistory(player);
             }
         }
     }

     //------------------------------------------------------------------------------------------------
     override void OnDelete(IEntity owner)
     {
         DisconnectFromNotificationsSystem();

         //~Hotfix to remember player names even when the player left
         if (Replication.IsClient())
         {
             SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
             if (gameMode)
                 gameMode.GetOnPlayerRegistered().Remove(AddPlayerNameToHistory);
         }

         super.OnDelete(owner);
     }
 }