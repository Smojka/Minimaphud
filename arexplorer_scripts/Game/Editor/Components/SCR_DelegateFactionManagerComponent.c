 [ComponentEditorProps(category: "GameScripted/FactionManager/Components", description: "")]
 class SCR_DelegateFactionManagerComponentClass: SCR_BaseFactionManagerComponentClass
 {
 };
 //Here track tasks and spawn points
 //Find faction deletegate on changed send that to the correct faction then have an rpl function to send the task

 class SCR_DelegateFactionManagerComponent : SCR_BaseFactionManagerComponent
 {
     [Attribute(params: "et")]
     protected ResourceName m_FactionDelegatePrefab;

     protected ref map<Faction, SCR_EditableFactionComponent> m_FactionDelegates = new map<Faction, SCR_EditableFactionComponent>;


     static SCR_DelegateFactionManagerComponent GetInstance()
     {
         FactionManager factionManager = GetGame().GetFactionManager();
         if (factionManager)
             return SCR_DelegateFactionManagerComponent.Cast(factionManager.FindComponent(SCR_DelegateFactionManagerComponent));
         else
             return null;
     }
     int GetFactionDelegates(notnull out map<Faction, SCR_EditableFactionComponent> outDelegates)
     {
         int count = m_FactionDelegates.Count();
         for (int i = 0; i < count; i++)
         {
             outDelegates.Insert(m_FactionDelegates.GetKey(i), m_FactionDelegates.GetElement(i));
         }
         return count;
     }
     int GetSortedFactionDelegates(notnull out SCR_SortedArray<SCR_EditableFactionComponent> outDelegates)
     {
         int count = m_FactionDelegates.Count();
         SCR_Faction faction;
         SCR_EditableFactionComponent factionDelegate;
         for (int i = 0; i < count; i++)
         {
             factionDelegate = m_FactionDelegates.GetElement(i);
             int order = 0;
             faction = SCR_Faction.Cast(factionDelegate.GetFaction());
             if (faction)
                 order = faction.GetOrder();

             outDelegates.Insert(order, factionDelegate);
         }
         return count;
     }
     int GetFactionDelegateCount()
     {
         return m_FactionDelegates.Count();
     }
     int GetPlayableFactionDelegates(notnull out map<Faction, SCR_EditableFactionComponent> outDelegates)
     {
         foreach (Faction faction, SCR_EditableFactionComponent delegate: m_FactionDelegates)
         {
             SCR_Faction scrFaction = SCR_Faction.Cast(faction);
             if (scrFaction && scrFaction.IsPlayable())
             {
                 outDelegates.Insert(scrFaction, delegate);
             }
         }

         return outDelegates.Count();
     }
     int GetPlayableFactionDelegateCount()
     {
         int count = 0;

         foreach (Faction faction, SCR_EditableFactionComponent delegate: m_FactionDelegates)
         {
             SCR_Faction scrFaction = SCR_Faction.Cast(faction);
             if (scrFaction && scrFaction.IsPlayable())
                 count++;
         }

         return count;
     }
     SCR_EditableFactionComponent GetFactionDelegate(Faction faction)
     {
         return m_FactionDelegates.Get(faction);
     }
     void SetFactionDelegate(Faction faction, SCR_EditableFactionComponent delegate)
     {
         m_FactionDelegates.Set(faction, delegate);
     }

     override void OnFactionsInit(array<Faction> factions)
     {
         if (SCR_Global.IsEditMode(GetOwner()) || factions.IsEmpty() || (Replication.IsClient() && Replication.Runtime()))
             return;

         Resource entityResource = Resource.Load(m_FactionDelegatePrefab);
         IEntityComponentSource componentSource = SCR_EditableFactionComponentClass.GetEditableEntitySource(entityResource);
         if (!componentSource)
         {
             Print(string.Format("SCR_EditableFactionComponent missing in '%1'!", m_FactionDelegatePrefab.GetPath()), LogLevel.WARNING);
             return;
         }

         SCR_EditableFactionComponent editableEntity;
         foreach (int i, Faction faction: factions)
         {
             IEntity delegateEntity = GetGame().SpawnEntityPrefab(entityResource);
             editableEntity = SCR_EditableFactionComponent.Cast(SCR_EditableFactionComponent.GetEditableEntity(delegateEntity));
             editableEntity.SetFactionIndex(i);
         }
     }
 };