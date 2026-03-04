 [EntityEditorProps(category: "GameScripted/GameMode", description: "Area that provides events and API when an enemy enters it.")]
 class SCR_SpawnAreaClass : ScriptedGameTriggerEntityClass
 {
 };

 void SpawnAreaCharacterEventDelegate(SCR_SpawnArea area, IEntity character, bool isFriendly);
 typedef func SpawnAreaCharacterEventDelegate;
 typedef ScriptInvokerBase<SpawnAreaCharacterEventDelegate> SpawnAreaEvent;

 void SpawnAreaAlertEventDelegate(SCR_SpawnArea area, array<IEntity> enemies);
 typedef func SpawnAreaAlertEventDelegate;
 typedef ScriptInvokerBase<SpawnAreaAlertEventDelegate> SpawnAreaAlertEvent;

 //------------------------------------------------------------------------------------------------
 class SCR_SpawnArea : ScriptedGameTriggerEntity
 {
     [Attribute("", UIWidgets.EditBox, "The key specifying which faction this area belongs to.")]
     protected FactionKey m_sFactionKey;

     protected ref set<SCR_ChimeraCharacter>> m_sOccupants = new set<SCR_ChimeraCharacter>();

     protected ref SpawnAreaEvent m_pOnCharacterEnter = new SpawnAreaEvent();

     protected ref array<IEntity> m_aEnemies = {};

     //------------------------------------------------------------------------------------------------
  SpawnAreaEvent GetCharacterEnterInvoker()
     {
         return m_pOnCharacterEnter;
     }

     protected ref SpawnAreaEvent m_pOnCharacterExit = new SpawnAreaEvent();

     //------------------------------------------------------------------------------------------------
  SpawnAreaEvent GetCharacterExitInvoker()
     {
         return m_pOnCharacterExit;
     }

     protected ref SpawnAreaAlertEvent m_pOnAlert = new SpawnAreaAlertEvent();

     //------------------------------------------------------------------------------------------------
  SpawnAreaAlertEvent GetOnAlertInvoker()
     {
         return m_pOnAlert;
     }

     //------------------------------------------------------------------------------------------------
  Faction GetAffiliatedFaction()
     {
         return GetGame().GetFactionManager().GetFactionByKey(GetAffiliatedFactionKey());
     }

     //------------------------------------------------------------------------------------------------
     FactionKey GetAffiliatedFactionKey()
     {
         return m_sFactionKey;
     }

     //------------------------------------------------------------------------------------------------
  bool IsInside(SCR_ChimeraCharacter character)
     {
         if (!character)
             return false;

         return m_sOccupants.Contains(character);
     }

     //------------------------------------------------------------------------------------------------
  bool IsFriendly(notnull SCR_ChimeraCharacter character)
     {
         Faction faction = character.GetFaction();
         if (!faction)
         {
             if (m_sFactionKey.IsEmpty())
                 return true;

             return false;
         }

         Faction areaFaction = GetAffiliatedFaction();
         if (areaFaction && areaFaction.IsFactionFriendly(faction))
             return true;

         return false;
     }


     //------------------------------------------------------------------------------------------------
  int GetFriendlyCharactersInside(out notnull array<SCR_ChimeraCharacter> outCharacters)
     {
         outCharacters.Clear();
         int count = 0;
         foreach (SCR_ChimeraCharacter character : m_sOccupants)
         {
             if (!character)
                 continue;

             if (IsFriendly(character))
             {
                 outCharacters.Insert(character);
                 count++;
             }
         }

         return count;
     }

     //------------------------------------------------------------------------------------------------
  int GetEnemyCharactersInside(out notnull array<SCR_ChimeraCharacter> outCharacters)
     {
         outCharacters.Clear();
         int count = 0;
         foreach (SCR_ChimeraCharacter character : m_sOccupants)
         {
             if (!character)
                 continue;

             if (!IsFriendly(character))
             {
                 outCharacters.Insert(character);
                 count++;
             }
         }

         return count;
     }


     //------------------------------------------------------------------------------------------------
  int GetCharactersInside(out notnull array<SCR_ChimeraCharacter> outCharacters)
     {
         outCharacters.Clear();
         int count = 0;
         foreach (SCR_ChimeraCharacter character : m_sOccupants)
         {
             outCharacters.Insert(character);
             count++;
         }

         return count;
     }

     //------------------------------------------------------------------------------------------------
  override bool ScriptedEntityFilterForQuery(IEntity ent)
     {
         SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
         if (!character)
             return false;

         return !character.GetCharacterController().IsDead();
     }

     //------------------------------------------------------------------------------------------------
     protected bool IsLocalEntity(notnull IEntity ent)
     {
         PlayerController playerController = GetGame().GetPlayerController();
         if (playerController && playerController.GetControlledEntity() == ent)
             return true;

         return false;
     }

     //------------------------------------------------------------------------------------------------
  protected override void OnActivate(IEntity ent)
     {
         SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
         if (!character)
             return;

         // Push new character into occupants
         if (!m_sOccupants.Contains(character))
         {
             m_sOccupants.Insert(character);

             // Raise callback
             OnCharacterEnter(character, IsFriendly(character));
         }
     }

     //------------------------------------------------------------------------------------------------
     protected event void OnCharacterEnter(IEntity character, bool isFriendly)
     {
         m_pOnCharacterEnter.Invoke(this, character, isFriendly)
     }

     //------------------------------------------------------------------------------------------------
  protected override void OnDeactivate(IEntity ent)
     {
         SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
         if (!character)
             return;

         if (m_sOccupants.Contains(character))
         {
             int index = m_sOccupants.Find(character);
             m_sOccupants.Remove(index);

             OnCharacterExit(character, IsFriendly(character));
         }
     }

     //------------------------------------------------------------------------------------------------
     protected event void OnCharacterExit(IEntity character, bool isFriendly)
     {
         m_pOnCharacterExit.Invoke(this, character, isFriendly)
     }

     //------------------------------------------------------------------------------------------------
  protected override void OnInit(IEntity owner)
     {
         // Enable OnFrame event mask
         SetEventMask(EntityEvent.FRAME);
     }

     //------------------------------------------------------------------------------------------------
     protected override void OnFrame(IEntity owner, float timeSlice)
     {
         super.OnFrame(owner, timeSlice);

         m_aEnemies.Clear();
         // Ignore dead characters and find if there is at least one enemy
         foreach (SCR_ChimeraCharacter character : m_sOccupants)
         {
             if (character.GetCharacterController().IsDead())
                 continue;

             if (!IsFriendly(character))
                 m_aEnemies.Insert(character);
         }

         // Raise alert event if any
         if (!m_aEnemies.IsEmpty())
             OnAlert(m_aEnemies);
     }

     //------------------------------------------------------------------------------------------------
     protected event void OnAlert(array<IEntity> enemies)
     {
         m_pOnAlert.Invoke(this, enemies);
     }
 };