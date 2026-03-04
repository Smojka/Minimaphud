 [ComponentEditorProps(category: "GameScripted/Respawn/PlayerController")]
 class SCR_PlayerFactionAffiliationComponentClass : SCR_FactionAffiliationComponentClass
 {
 }

 void PlayerFactionRequestDelegate(SCR_PlayerFactionAffiliationComponent component, int factionIndex);
 typedef func PlayerFactionRequestDelegate;
 typedef ScriptInvokerBase<PlayerFactionRequestDelegate> OnPlayerFactionRequestInvoker;

 void PlayerFactionResponseDelegate(SCR_PlayerFactionAffiliationComponent component, int factionIndex, bool response);
 typedef func PlayerFactionResponseDelegate;
 typedef ScriptInvokerBase<PlayerFactionResponseDelegate> OnPlayerFactionResponseInvoker;

 class SCR_PlayerFactionAffiliationComponent : SCR_FactionAffiliationComponent
 {
     private PlayerController m_PlayerController;
     private RplComponent m_RplComponent;
     private SCR_RespawnComponent m_RespawnComponent;
     private SCR_SpawnLockComponent m_Lock;

     //------------------------------------------------------------------------------------------------
     PlayerController GetPlayerController()
     {
         return m_PlayerController;
     }

     //------------------------------------------------------------------------------------------------
  int GetPlayerId()
     {
         return GetPlayerController().GetPlayerId();
     }

     //------------------------------------------------------------------------------------------------
  protected SCR_SpawnLockComponent GetLock()
     {
         return m_Lock;
     }

     // ON CAN FACTION REQUEST
     protected ref OnPlayerFactionRequestInvoker m_OnCanPlayerFactionRequestInvoker_O = new OnPlayerFactionRequestInvoker();

     //------------------------------------------------------------------------------------------------
  OnPlayerFactionRequestInvoker GetOnCanPlayerFactionRequestInvoker_O()
     {
         return m_OnCanPlayerFactionRequestInvoker_O;
     }

     protected ref OnPlayerFactionRequestInvoker m_OnCanPlayerFactionRequestInvoker_S = new OnPlayerFactionRequestInvoker();

     //------------------------------------------------------------------------------------------------
  OnPlayerFactionRequestInvoker GetOnCanPlayerFactionRequestInvoker_S()
     {
         return m_OnCanPlayerFactionRequestInvoker_S;
     }

     // ON CAN FACTION RESPONSE
     protected ref OnPlayerFactionResponseInvoker m_OnCanPlayerFactionResponseInvoker_O = new OnPlayerFactionResponseInvoker();

     //------------------------------------------------------------------------------------------------
  OnPlayerFactionResponseInvoker GetOnCanPlayerFactionResponseInvoker_O()
     {
         return m_OnCanPlayerFactionResponseInvoker_O;
     }

     protected ref OnPlayerFactionResponseInvoker m_OnCanPlayerFactionResponseInvoker_S = new OnPlayerFactionResponseInvoker();

     //------------------------------------------------------------------------------------------------
  OnPlayerFactionResponseInvoker GetOnCanPlayerFactionResponseInvoker_S()
     {
         return m_OnCanPlayerFactionResponseInvoker_S;
     }

     // ON FACTION REQUEST
     protected ref OnPlayerFactionRequestInvoker m_OnPlayerFactionRequestInvoker_O = new OnPlayerFactionRequestInvoker();

     //------------------------------------------------------------------------------------------------
  OnPlayerFactionRequestInvoker GetOnPlayerFactionRequestInvoker_O()
     {
         return m_OnPlayerFactionRequestInvoker_O;
     }

     protected ref OnPlayerFactionRequestInvoker m_OnPlayerFactionRequestInvoker_S = new OnPlayerFactionRequestInvoker();

     //------------------------------------------------------------------------------------------------
  OnPlayerFactionRequestInvoker GetOnPlayerFactionRequestInvoker_S()
     {
         return m_OnPlayerFactionRequestInvoker_S;
     }

     //------------------------------------------------------------------------------------------------
     // ON FACTION RESPONSE
     protected ref OnPlayerFactionResponseInvoker m_OnPlayerFactionResponseInvoker_O = new OnPlayerFactionResponseInvoker();

     //------------------------------------------------------------------------------------------------
  OnPlayerFactionResponseInvoker GetOnPlayerFactionResponseInvoker_O()
     {
         return m_OnPlayerFactionResponseInvoker_O;
     }
     protected ref OnPlayerFactionResponseInvoker m_OnPlayerFactionResponseInvoker_S = new OnPlayerFactionResponseInvoker();
     //------------------------------------------------------------------------------------------------
  OnPlayerFactionResponseInvoker GetOnPlayerFactionResponseInvoker_S()
     {
         return m_OnPlayerFactionResponseInvoker_S;
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsOwner()
     {
         return !m_RplComponent || m_RplComponent.IsOwner();
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsProxy()
     {
         return m_RplComponent && m_RplComponent.IsProxy();
     }

     //------------------------------------------------------------------------------------------------
     protected override void OnPostInit(IEntity owner)
     {
         super.OnPostInit(owner);
         m_PlayerController = PlayerController.Cast(owner);
         if (!m_PlayerController)
             Debug.Error(string.Format("%1 is not attached to a %2", Type().ToString(), PlayerController));

         m_RespawnComponent = SCR_RespawnComponent.Cast(m_PlayerController.GetRespawnComponent());
         m_Lock = SCR_SpawnLockComponent.Cast(owner.FindComponent(SCR_SpawnLockComponent));
         m_RplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));

         #ifdef ENABLE_DIAG
         DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_PLAYER_FACTION_DIAG, "", "Player Factions", "GameMode");
         GetGame().GetCallqueue().CallLater(OnDiag, 0, true);
         #endif
     }

     //------------------------------------------------------------------------------------------------
  bool RequestFaction(Faction faction)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::RequestFaction(faction: %2)", Type().ToString(), faction), LogLevel.NORMAL);
         #endif

         // Lock this
         int factionIndex = GetGame().GetFactionManager().GetFactionIndex(faction);
         SCR_SpawnLockComponent lock = GetLock();
         if (lock && !lock.TryLock(this, false))
         {
             Debug.Error("Caught request on locked player!");
             return false;
         }

         // Notify owner
         if (IsOwner())
             GetOnPlayerFactionRequestInvoker_O().Invoke(this, factionIndex);

         if (!IsProxy())
             GetOnPlayerFactionRequestInvoker_S().Invoke(this, factionIndex);

         Rpc(Rpc_RequestFaction_S, factionIndex);
         return true;
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void Rpc_RequestFaction_S(int factionIndex)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::Rpc_RequestFaction_S(factionIdx: %2)", Type().ToString(), factionIndex), LogLevel.NORMAL);
         #endif

         // Lock server
         SCR_SpawnLockComponent lock = GetLock();
         if (lock && !lock.TryLock(this, true))
         {
             Debug.Error("Caught request on locked player!");
             return;
         }

         // Notify server
         GetOnPlayerFactionRequestInvoker_S().Invoke(this, factionIndex);

         Faction faction = GetGame().GetFactionManager().GetFactionByIndex(factionIndex);
         if (CanRequestFaction_S(faction))
         {
             if (SetFaction_S(faction))
             {
                 // Respond
                 SendRequestFactionResponse_S(factionIndex, true);
                 return;
             }
         }

         // Failure
         SendRequestFactionResponse_S(factionIndex, false);
     }

     //------------------------------------------------------------------------------------------------
  protected bool SetFaction_S(Faction faction)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::RequestFaction_S(Faction: %2)", Type().ToString(), faction), LogLevel.NORMAL);
         #endif

         // Assign faction
         SetAffiliatedFaction(faction);
         if (GetAffiliatedFaction() == faction)
         {
             // Update faction manager
             SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
             factionManager.UpdatePlayerFaction_S(this);

             // Notify game mode
             SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
             gameMode.OnPlayerFactionSet_S(this, faction);
             return true;
         }

         return false;
     }

     //------------------------------------------------------------------------------------------------
  protected void SendRequestFactionResponse_S(int factionIndex, bool response)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::SendRequestFactionResponse_S(factionIdx: %2, response: %3)", Type().ToString(), factionIndex, response), LogLevel.NORMAL);
         #endif

         // Unlock server
         SCR_SpawnLockComponent lock = GetLock();
         if (lock)
         {
             lock.Unlock(this, true);
             // And unlock its "requester/proxy" side in case request came from self
             lock.Unlock(this, false);
         }

         // Notify this
         GetOnPlayerFactionResponseInvoker_S().Invoke(this, factionIndex, response);

         // Notify owner
         Rpc(RequestFactionResponse_O, factionIndex, response);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     protected void RequestFactionResponse_O(int factionIndex, bool response)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::RequestFactionResponse_O(factionIdx: %2, response: %3)", Type().ToString(), factionIndex, response), LogLevel.NORMAL);
         #endif

         // Unlock this
         SCR_SpawnLockComponent lock = GetLock();
         if (lock)
             lock.Unlock(this, false);

         // Notify this
         GetOnPlayerFactionResponseInvoker_O().Invoke(this, factionIndex, response);
     }

     //------------------------------------------------------------------------------------------------
  bool CanRequestFaction(Faction faction)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format(Format("%1::CanRequestFaction(faction: %2)", Type().ToString(), faction), LogLevel.NORMAL);
         #endif

         // Lock this
         SCR_SpawnLockComponent lock = GetLock();
         if (lock && !lock.TryLock(this, false))
         {
             Debug.Error("Caught request on locked player!");
             return false;
         }

         int factionIndex = GetGame().GetFactionManager().GetFactionIndex(faction);

         // Notify owner
         if (IsOwner())
             GetOnCanPlayerFactionRequestInvoker_O().Invoke(this, factionIndex);

         // Notify au
         if (!IsProxy())
             GetOnCanPlayerFactionRequestInvoker_S().Invoke(this, factionIndex);

         Rpc(Rpc_CanRequestFaction_S, factionIndex);
         return true;
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Server)]
     protected void Rpc_CanRequestFaction_S(int factionIndex)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::Rpc_CanRequestFaction_S(factionIdx: %2)", Type().ToString(), factionIndex), LogLevel.NORMAL);
         #endif

         // Lock server
         SCR_SpawnLockComponent lock = GetLock();
         if (lock && !lock.TryLock(this, true))
         {
             Debug.Error("Caught request on locked player!");
             return;
         }

         // Notify server
         GetOnCanPlayerFactionRequestInvoker_S().Invoke(this, factionIndex);

         Faction faction = GetGame().GetFactionManager().GetFactionByIndex(factionIndex);
         if (!CanRequestFaction_S(faction))
         {
             SendCanRequestFactionResponse_S(factionIndex, false);
             return;
         }

         SendCanRequestFactionResponse_S(factionIndex, true);
     }

     //------------------------------------------------------------------------------------------------
  protected bool CanRequestFaction_S(Faction faction)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::CanRequestFaction_S(Faction: %2)", Type().ToString(), faction), LogLevel.NORMAL);
         #endif

         // Do not allow to set faction to faction that is already set,
         // that will just lead to spam and invokes of irrelevant stuff
         if (GetAffiliatedFaction() == faction)
             return false;

         // Any arbitrary logic
         return true;
     }

     //------------------------------------------------------------------------------------------------
  protected void SendCanRequestFactionResponse_S(int factionIndex, bool response)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::SendCanRequestFactionResponse_S(factionIdx: %2, response: %3)", Type().ToString(), factionIndex, response), LogLevel.NORMAL);
         #endif

         // Unlock server
         SCR_SpawnLockComponent lock = GetLock();
         if (lock)
         {
             lock.Unlock(this, true);
             // Unlock the server's request ("proxy") side too
             lock.Unlock(this, false);
         }

         // Notify owner
         GetOnCanPlayerFactionResponseInvoker_S().Invoke(this, factionIndex, response);

         // Notify user
         Rpc(CanRequestFactionResponse_O, factionIndex, response);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Owner)]
     protected void CanRequestFactionResponse_O(int factionIndex, bool response)
     {
         #ifdef _ENABLE_RESPAWN_LOGS
         Print(string.Format("%1::CanRequestFactionResponse_O(factionIdx: %2, response: %3)", Type().ToString(), factionIndex, response), LogLevel.NORMAL);
         #endif

         // Unlock this
         SCR_SpawnLockComponent lock = GetLock();
         if (lock)
             lock.Unlock(this, false);

         // Notify owner
         GetOnCanPlayerFactionResponseInvoker_O().Invoke(this, factionIndex, response);
     }

     #ifdef ENABLE_DIAG
     //------------------------------------------------------------------------------------------------
  protected void OnDiag()
     {
         if (!DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_PLAYER_FACTION_DIAG))
             return;

         int playerId = GetPlayerController().GetPlayerId();
         DbgUI.Begin(string.Format("PlayerFaction (id: %1)", playerId));
         {
             FactionKey factionKey = "Unassigned";

             Faction faction = GetAffiliatedFaction();
             if (faction)
                 factionKey = faction.GetFactionKey();

             DbgUI.Text(string.Format("Current: %1 (%2)", factionKey, faction));

             FactionManager factionManager = GetGame().GetFactionManager();
             string availableFactions = "Available Factions: ";
             array<Faction> factions = {};
             factionManager.GetFactionsList(factions);
             if (factions.Count() > 0)
             {
                 string fk = factions[0].GetFactionKey();
                 availableFactions += fk;

                 for (int i = 1; i < factions.Count(); i++)
                 {
                     fk = factions[i].GetFactionKey();
                     availableFactions += string.Format(", %1", fk);
                 }
             }

             if (availableFactions.IsEmpty())
                 availableFactions += "None";

             DbgUI.Text(availableFactions);

             string wanted;
             DbgUI.InputText("Wanted Faction Key", wanted);

             Faction wantedFaction = factionManager.GetFactionByKey(wanted);
             if (wantedFaction)
             {
                 DbgUI.Text(string.Format("Wanted: %1 (%2)", wantedFaction.GetFactionKey(), wantedFaction));
                 if (DbgUI.Button("CanRequest"))
                     CanRequestFaction(wantedFaction);
                 if (DbgUI.Button("Request"))
                     RequestFaction(wantedFaction);
             }
         }
         DbgUI.End();
     }
     #endif
 }