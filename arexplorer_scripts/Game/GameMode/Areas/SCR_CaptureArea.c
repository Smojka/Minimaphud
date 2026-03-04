 [EntityEditorProps(category: "GameScripted/GameMode", description: "Area that provides basic events and api to serve as a captureable area.")]
 class SCR_CaptureAreaClass : ScriptedGameTriggerEntityClass
 {
 };

 void CaptureAreaCharacterEventDelegate(SCR_CaptureArea area, Faction affiliatedFaction, IEntity character);
 typedef func CaptureAreaCharacterEventDelegate;
 typedef ScriptInvokerBase<CaptureAreaCharacterEventDelegate> CaptureAreaEvent;

 void CaptureAreaOwnerFactionEventDelegate(SCR_CaptureArea area, Faction previousOwner, Faction newOwner);
 typedef func CaptureAreaOwnerFactionEventDelegate;
 typedef ScriptInvokerBase<CaptureAreaOwnerFactionEventDelegate> CaptureAreaOwnershipEvent;

 //------------------------------------------------------------------------------------------------
 class SCR_CaptureArea : ScriptedGameTriggerEntity
 {
     protected ref map<Faction, ref array<SCR_ChimeraCharacter>> m_mOccupants = new map<Faction, ref array<SCR_ChimeraCharacter>>();

     protected ref CaptureAreaEvent m_pOnCharacterEnter = new CaptureAreaEvent();

     //------------------------------------------------------------------------------------------------
  CaptureAreaEvent GetCharacterEnterInvoker()
     {
         return m_pOnCharacterEnter;
     }

     protected ref CaptureAreaEvent m_pOnCharacterExit = new CaptureAreaEvent();

     //------------------------------------------------------------------------------------------------
  CaptureAreaEvent GetCharacterExitInvoker()
     {
         return m_pOnCharacterExit;
     }

     protected ref CaptureAreaOwnershipEvent m_pOnOwnershipChanged = new CaptureAreaOwnershipEvent();

     //------------------------------------------------------------------------------------------------
  CaptureAreaOwnershipEvent GetOwnershipChangedEvent()
     {
         return m_pOnOwnershipChanged;
     }

     protected Faction m_pOwnerFaction;
     Faction GetOwningFaction() { return m_pOwnerFaction; }

     protected RplComponent m_pRplComponent;

     //------------------------------------------------------------------------------------------------
  protected override void OnInit(IEntity owner)
     {
         // Do not spam messages outside of playmode,
         // these might not be relevant (yet) also
         // there is no need to initialize the entity (just yet)
         if (!GetGame().InPlayMode())
             return;

         // Mandatory, cannot work without factions
         FactionManager factionManager = GetGame().GetFactionManager();
         if (!factionManager)
         {
             Debug.Error("No faction manager present in the world! Capture area will malfunction!");
             return;
         }

         // Neccessary to determine replication logic
         m_pRplComponent = RplComponent.Cast(FindComponent(RplComponent));
         if (!m_pRplComponent)
         {
             Debug.Error("SCR_CaptureArea requires RplComponent to function!");
             return;
         }

         // Enable OnFrame event mask
         SetEventMask(EntityEvent.FRAME);

         array<Faction> availableFactions = {};
         factionManager.GetFactionsList(availableFactions);

         // Fetch available actions and preallocate map arrays
         foreach (Faction faction : availableFactions)
             m_mOccupants.Insert(faction, new array<SCR_ChimeraCharacter>());
     }

     protected override bool RplLoad(ScriptBitReader reader)
     {
         int factionIndex = -1;
         reader.ReadInt(factionIndex);

         Faction ownerFaction;
         if (factionIndex != -1)
             ownerFaction = GetGame().GetFactionManager().GetFactionByIndex(factionIndex);

         m_pOwnerFaction = ownerFaction;
         return true;
     }

     protected override bool RplSave(ScriptBitWriter writer)
     {
         int factionIndex = -1;
         if (m_pOwnerFaction)
             factionIndex = GetGame().GetFactionManager().GetFactionIndex(m_pOwnerFaction);

         writer.WriteInt(factionIndex);
         return true;
     }

     //------------------------------------------------------------------------------------------------
  protected override bool ScriptedEntityFilterForQuery(IEntity ent)
     {
         // Filter for characters only
         SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
         if (!character)
             return false;

         // That are alive.
         return !character.GetCharacterController().IsDead();
     }

     //------------------------------------------------------------------------------------------------
  protected override void OnActivate(IEntity ent)
     {
         if (!m_mOccupants)
             return;

         SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
         if (!character)
             return;

         Faction faction = character.GetFaction();
         if (!faction)
             return;

         if (faction && !m_mOccupants[faction].Contains(character))
         {
             m_mOccupants[faction].Insert(character);
             OnCharacterEntered(faction, character);
         }
     }

     //------------------------------------------------------------------------------------------------
  protected event void OnCharacterEntered(Faction faction, SCR_ChimeraCharacter character)
     {
         m_pOnCharacterEnter.Invoke(this, faction, character);
     }

     //------------------------------------------------------------------------------------------------
  protected override void OnDeactivate(IEntity ent)
     {
         SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
         Faction faction = character.GetFaction();

         if (faction)
         {
             m_mOccupants[faction].RemoveItem(character);
             OnCharacterExit(faction, character);
         }
     }

     //------------------------------------------------------------------------------------------------
  protected event void OnCharacterExit(Faction faction, SCR_ChimeraCharacter character)
     {
         m_pOnCharacterExit.Invoke(this, faction, character);
     }

     //------------------------------------------------------------------------------------------------
  int GetOccupants(Faction faction, notnull array<SCR_ChimeraCharacter> outCharacters)
     {
         array<SCR_ChimeraCharacter> characters = m_mOccupants[faction];
         if (!characters || characters.IsEmpty())
             return 0;

         outCharacters.Copy(characters);
         return characters.Count();
     }

     //------------------------------------------------------------------------------------------------
  int GetOccupantsCount(Faction faction)
     {
         return m_mOccupants[faction].Count();
     }

     //------------------------------------------------------------------------------------------------
  protected Faction EvaluateOwnerFaction()
     {
         // Fetch all available factions
         array<Faction> availableFactions = {};
         GetGame().GetFactionManager().GetFactionsList(availableFactions);

         int maxCount;
         Faction maxFaction;
         int occupantsCount;
         foreach (Faction faction : availableFactions)
         {
             occupantsCount = GetOccupantsCount(faction);
             if (occupantsCount == 0)
                 continue;

             if (occupantsCount > maxCount)
             {
                 maxCount = occupantsCount;
                 maxFaction = faction;
             }
         }

         // With no alive occupants in the area, no faction can
         // be deemed as the capturing one
         if (maxCount == 0)
             return null;

         return maxFaction;
     }

     //------------------------------------------------------------------------------------------------
  protected override event void OnFrame(IEntity owner, float timeSlice)
     {
         super.OnFrame(owner, timeSlice);

         // Since trigger can be out of sync with character states,
         // filter out dead characters if any are left in the collection
         foreach (Faction faction, array<SCR_ChimeraCharacter> characters : m_mOccupants)
         {
             for (int i = characters.Count() - 1; i >= 0; --i)
             {
                 SCR_ChimeraCharacter occupant = characters[i];
                 if (!occupant || occupant.GetCharacterController().IsDead())
                 {
                     characters.Remove(i);
                     continue;
                 }
             }
         }

         // Only the authority will be updating the state,
         // rpl component is prerequisite for this entity
         if (!m_pRplComponent || !m_pRplComponent.IsMaster())
             return;

         Faction newOwner = EvaluateOwnerFaction();
         if (newOwner != m_pOwnerFaction)
         {
             Faction previousOwner = m_pOwnerFaction;
             SetOwningFactionInternal(previousOwner, newOwner);

             // For the authority, this is fired straight away above,
             // so we only send the change to all clients as broadcast
             FactionManager factionManager = GetGame().GetFactionManager();
             int previousIndex = factionManager.GetFactionIndex(previousOwner);
             int newIndex = factionManager.GetFactionIndex(newOwner);
             Rpc(Rpc_SetOwningFaction_BC, previousIndex, newIndex);
         }
     }

     //------------------------------------------------------------------------------------------------
  protected void SetOwningFactionInternal(Faction previousFaction, Faction newFaction)
     {
         m_pOwnerFaction = newFaction;
         OnOwningFactionChanged(previousFaction, newFaction);
     }

     //------------------------------------------------------------------------------------------------
     [RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
     protected void Rpc_SetOwningFaction_BC(int previousFactionIndex, int newFactionIndex)
     {
         FactionManager factionManager = GetGame().GetFactionManager();

         Faction previousFaction;
         if (previousFactionIndex != -1)
             previousFaction = factionManager.GetFactionByIndex(previousFactionIndex);

         Faction newFaction;
         if (newFactionIndex != -1)
             newFaction = factionManager.GetFactionByIndex(newFactionIndex);

         SetOwningFactionInternal(previousFaction, newFaction);
     }

     //------------------------------------------------------------------------------------------------
  protected event void OnOwningFactionChanged(Faction previousFaction, Faction newFaction)
     {
         m_pOnOwnershipChanged.Invoke(this, previousFaction, newFaction);
     }
 };