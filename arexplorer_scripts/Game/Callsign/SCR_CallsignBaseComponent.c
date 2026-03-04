 [ComponentEditorProps(category: "GameScripted/Callsign", description: "")]
 class SCR_CallsignBaseComponentClass : ScriptComponentClass
 {
 }

 //~ ScriptInvokers
 void SCR_CallsignBaseComponent_OnCallsignChanged(int companyIndex, int platoonIndex, int squadIndex, int characterNumber, ERoleCallsign characterRole);
 typedef func SCR_CallsignBaseComponent_OnCallsignChanged;

 class SCR_CallsignBaseComponent : ScriptComponent
 {
     protected Faction m_Faction;
     protected SCR_FactionCallsignInfo m_CallsignInfo;

     //Callsign IDs(Broadcast)
     protected int m_iCompanyCallsign = -1;
     protected int m_iPlatoonCallsign = -1;
     protected int m_iSquadCallsign = -1;

     //State
     protected bool m_bIsServer;

     //Ref
     protected SCR_CallsignManagerComponent m_CallsignManager;

     //Will always return int Company, int Platoon, int squad, int character number and character role (See GetCallsignIndexes() comments for more information). Also called when assigning callsigns for first time
     protected ref ScriptInvokerBase<SCR_CallsignBaseComponent_OnCallsignChanged> Event_OnCallsignChanged = new ScriptInvokerBase<SCR_CallsignBaseComponent_OnCallsignChanged>();

     //------------------------------------------------------------------------------------------------
  bool GetCallsignNames(out string company, out string platoon, out string squad, out string character, out string format)
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool GetCallsignIndexes(out int companyIndex, out int platoonIndex, out int squadIndex, out int characterNumber = -1, out ERoleCallsign characterRole = ERoleCallsign.NONE)
     {
     }

     //------------------------------------------------------------------------------------------------
     protected void ClearCallsigns()
     {
         m_CallsignInfo = null;

         m_iCompanyCallsign = -1;
         m_iPlatoonCallsign = -1;
         m_iSquadCallsign = -1;
     }

     //------------------------------------------------------------------------------------------------
     protected bool SetCallsignInfo()
     {
         Faction currentFaction;

         SCR_AIGroup group = SCR_AIGroup.Cast(GetOwner());
         if (group && group.IsPlayable())
                 currentFaction = group.GetFaction();
         else
         {
             SCR_EditableEntityComponent editableEntityComponent = SCR_EditableEntityComponent.GetEditableEntity(GetOwner());
             if (editableEntityComponent)
                 currentFaction = editableEntityComponent.GetFaction();
         }

         if (m_Faction != currentFaction && currentFaction != null)
         {
             m_Faction = currentFaction;
             m_CallsignInfo = null;
         }
         else
         {
             if (m_CallsignInfo)
                 return true;
         }

         SCR_Faction scrFaction = SCR_Faction.Cast(m_Faction);

         if (!scrFaction)
             return false;

         m_CallsignInfo = scrFaction.GetCallsignInfo();
         return m_CallsignInfo != null;
     }

     //------------------------------------------------------------------------------------------------
     ScriptInvokerBase<SCR_CallsignBaseComponent_OnCallsignChanged> GetOnCallsignChanged()
     {
         return Event_OnCallsignChanged;
     }

     //------------------------------------------------------------------------------------------------
     protected void InitOnServer(IEntity owner)
     {
     }

     //------------------------------------------------------------------------------------------------
     override void EOnInit(IEntity owner)
     {
         if (SCR_Global.IsEditMode(owner))
             return;

         BaseGameMode gameMode = GetGame().GetGameMode();
         if (!gameMode)
             return;

         m_CallsignManager = SCR_CallsignManagerComponent.Cast(gameMode.FindComponent(SCR_CallsignManagerComponent));
         if (!m_CallsignManager)
             return;

         SCR_EditableEntityComponent editableEntityComponent = SCR_EditableEntityComponent.GetEditableEntity(owner);
         if (editableEntityComponent)
             m_Faction = editableEntityComponent.GetFaction();
         m_bIsServer = Replication.IsServer();

         if (!m_bIsServer)
             return;

         GetGame().GetCallqueue().CallLater(InitOnServer, 1, false, owner);
     }

     //------------------------------------------------------------------------------------------------
     override void OnPostInit(IEntity owner)
     {
         SetEventMask(owner, EntityEvent.INIT);
     }
 }