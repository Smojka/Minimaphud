 [ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
 class SCR_EditableWaypointComponentClass : SCR_EditableEntityComponentClass
 {
 }


 class SCR_EditableWaypointComponent : SCR_EditableEntityComponent
 {
     [RplProp(onRplName: "OnPreWaypointIdRpl")]
     protected RplId m_PrevWaypointId;

     [RplProp()]
     protected bool m_bIsCurrent;

     [RplProp()]
     protected int m_iIndex;

     protected SCR_EditableEntityComponent m_Group;
     protected SCR_EditableEntityComponent m_PrevWaypoint;

     //------------------------------------------------------------------------------------------------
  void SetWaypointIndex(int index, bool isCurrent, SCR_EditableEntityComponent prevWaypoint)
     {
         m_iIndex = index;
         m_bIsCurrent = isCurrent;
         m_PrevWaypoint = prevWaypoint;
         m_PrevWaypointId = Replication.FindId(prevWaypoint);
         Replication.BumpMe();
     }

     //------------------------------------------------------------------------------------------------
  int GetWaypointIndex()
     {
         return m_iIndex;
     }

     //------------------------------------------------------------------------------------------------
  bool IsCurrent()
     {
         return m_bIsCurrent;
     }

     //------------------------------------------------------------------------------------------------
  SCR_EditableEntityComponent GetPrevWaypoint()
     {
         if (m_PrevWaypoint)
             return m_PrevWaypoint;
         else
             return m_Group;
     }

     //------------------------------------------------------------------------------------------------
     protected void OnPreWaypointIdRpl()
     {
         m_PrevWaypoint = SCR_EditableEntityComponent.Cast(Replication.FindItem(m_PrevWaypointId));
     }

     //------------------------------------------------------------------------------------------------
     override bool CanDuplicate(out notnull set<SCR_EditableEntityComponent> outRecipients)
     {
         SCR_EditableEntityComponent groupComponent = GetAIGroup();
         if (groupComponent)
             outRecipients.Insert(groupComponent);

         return true;
     }

     //------------------------------------------------------------------------------------------------
     override Faction GetFaction()
     {
         if (!m_Group)
             return null;

         Faction faction = m_Group.GetFaction();
         return faction;
     }

     //------------------------------------------------------------------------------------------------
     override SCR_EditableEntityComponent GetAIGroup()
     {
         return m_Group;
     }

     //------------------------------------------------------------------------------------------------
     override void OnParentEntityChanged(SCR_EditableEntityComponent parentEntity, SCR_EditableEntityComponent parentEntityPrev, bool changedByUser)
     {
         if (!parentEntity)
             return;

         EEditableEntityType parentType = parentEntity.GetEntityType();
         switch (parentType)
         {
             case EEditableEntityType.GROUP:
             case EEditableEntityType.CHARACTER:
             {
                 SCR_EditableEntityComponent group = parentEntity.GetAIGroup();
                 if (!group) break;

                 m_Group = group;
                 super.OnParentEntityChanged(group, parentEntityPrev, changedByUser);
                 break;
             }
         }
     }

     //------------------------------------------------------------------------------------------------
     override bool GetPos(out vector pos)
     {
         if (m_Group)
             return super.GetPos(pos);
         else
             return false;
     }

     //------------------------------------------------------------------------------------------------
     override SCR_EditableEntityComponent EOnEditorPlace(out SCR_EditableEntityComponent parent, SCR_EditableEntityComponent recipient, EEditorPlacingFlags flags, bool isQueue, int playerID = 0)
     {
         if (recipient)
         {
             AIGroup group = AIGroup.Cast(recipient.GetOwner());
             if (group)
             {
                 if (!isQueue)
                 {
                     SCR_EditableGroupComponent editableGroup = SCR_EditableGroupComponent.Cast(recipient);
                     if (editableGroup)
                         editableGroup.ClearWaypoints();
                 }

                 group.AddWaypoint(AIWaypoint.Cast(GetOwner()));
             }
         }
         return this;
     }

     //------------------------------------------------------------------------------------------------
     override void OnDelete(IEntity owner)
     {
         super.OnDelete(owner);

         //--- Manually remove the waypoint entity from group's list of waypoints - it doesn't happen automatically
         if (m_Group && IsServer())
         {
             AIGroup aiGroup = AIGroup.Cast(m_Group.GetOwner());
             if (aiGroup)
                 aiGroup.RemoveWaypoint(AIWaypoint.Cast(owner));
         }
     }
 }