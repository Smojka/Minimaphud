
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationDisplayData
 {
     [Attribute("0", UIWidgets.SearchComboBox, "Notification", "", ParamEnumArray.FromEnum(ENotification) )]
     ENotification m_NotificationKey;

     [Attribute()]
     bool m_bPriorityNotification;

     [Attribute(desc: "Holds the display information of the notification, Fill in Name and Color. Optional: Icon")]
     ref SCR_UINotificationInfo m_info;

     //Max amount of Characters a username can be. (Xbox = 12, Playstation = 16, Steam = 32)
     const int MAX_USERNAME_CHARACTERS = 32;

     //------------------------------------------------------------------------------------------------
  void SetInitialDisplayData(SCR_NotificationData data)
     {
         //Save Initial positions
         if (m_info.GetEditorSetPositionData() != ENotificationSetPositionData.NEVER_AUTO_SET_POSITION)
             SetPosition(data);

         //Save faction related color. Is ignored if no faction related color type is assigned
         SetFactionRelatedColor(data);
     }

     //------------------------------------------------------------------------------------------------
  void GetDisplayVisualizationData(SCR_NotificationData data, out SCR_UINotificationInfo info = null, out ENotificationColor colorEnum = ENotificationColor.NEUTRAL)
     {
         info = m_info;

         colorEnum = m_info.GetNotificationColor();

         if (colorEnum >= ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
             colorEnum = data.GetFactionRelatedColor();
     }

     //------------------------------------------------------------------------------------------------
  ENotificationColor GetTextColor(SCR_NotificationData data)
     {
         SCR_ColoredTextNotificationUIInfo coloredTextInfo = SCR_ColoredTextNotificationUIInfo.Cast(m_info);
         if (!coloredTextInfo)
             return ENotificationColor.NEUTRAL;

         ENotificationColor colorEnum = coloredTextInfo.GetNotificationTextColor();

         if (colorEnum >= ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
             colorEnum = data.GetFactionRelatedTextColor();

         return colorEnum;
     }

     //------------------------------------------------------------------------------------------------
  void GetSplitNotificationTextColors(SCR_NotificationData data, out ENotificationColor leftTextColorEnum = ENotificationColor.NEUTRAL, out ENotificationColor rightTextColorEnum = ENotificationColor.NEUTRAL)
     {
         SCR_SplitNotificationUIInfo splitNotificationUIInfo = SCR_SplitNotificationUIInfo.Cast(m_info);

         if (!splitNotificationUIInfo)
             return;

         ENotificationColor leftFactionColor, rightFactionColor;
         data.GetSplitFactionRelatedColor(leftFactionColor, rightFactionColor);

         leftTextColorEnum = splitNotificationUIInfo.GetLeftTextColor();

         if (leftTextColorEnum >= ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE || splitNotificationUIInfo.ShouldReplaceLeftColorWithRightColorIfAlly())
             leftTextColorEnum = leftFactionColor;

         rightTextColorEnum = splitNotificationUIInfo.GetRightTextColor();

         if (rightTextColorEnum >= ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
             rightTextColorEnum = rightFactionColor;
     }

     //------------------------------------------------------------------------------------------------
  string GetText(SCR_NotificationData data)
     {
         if (!m_info)
         {
             Print("(" + typename.EnumToString(ENotification, data.GetID()) + ") SCR_NotificationDisplayData has no UIInfo assigned!", LogLevel.WARNING);
             return string.Empty;
         }

         return m_info.GetName();
     }

     //------------------------------------------------------------------------------------------------
  bool MergeParam1With2()
     {
         return false;
     }

     //------------------------------------------------------------------------------------------------
  bool GetPriority()
     {
         return m_bPriorityNotification;
     }

     //------------------------------------------------------------------------------------------------
  SCR_UINotificationInfo GetNotificationUIInfo()
     {
         return m_info;
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetPlayerName(int playerID, out string playerName)
     {
         //~ Name already assigned
         if (!playerName.IsEmpty())
             return true;

         PlayerManager playerManager = GetGame().GetPlayerManager();
         if (!playerManager)
             return false;

         playerName = playerManager.GetPlayerName(playerID);

         //Player name not found
         if (playerName.IsEmpty())
         {
             SCR_NotificationsComponent notificationsManager = SCR_NotificationsComponent.GetInstance();
             if (notificationsManager)
                 playerName = notificationsManager.GetPlayerNameFromHistory(playerID);
         }

         if (playerName.Length() > MAX_USERNAME_CHARACTERS)
         {
             string trimedName = playerName.Substring(0, MAX_USERNAME_CHARACTERS);
             playerName = trimedName + "...";
         }

         return !playerName.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetEditableEntityName(int entityRplID, out string entityName, bool useCharacterName = false)
     {
         //~ Name already assigned
         if (!entityName.IsEmpty())
             return true;

         //Get target Entity. Also works with non-editable entity iD
         SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(Replication.FindItem(entityRplID));
         if (!editableEntity)
         {
             IEntity entity = IEntity.Cast(Replication.FindItem(entityRplID));
             if (!entity)
             {
                 RplComponent rplComponent = RplComponent.Cast(Replication.FindItem(entityRplID));
                 if (rplComponent)
                     entity = rplComponent.GetEntity();
             }
             if (entity)
                 editableEntity = SCR_EditableEntityComponent.Cast(entity.FindComponent(SCR_EditableEntityComponent));
         }

         //~ Editable entity component
         if (editableEntity)
         {
             if (editableEntity.GetEntityType() != EEditableEntityType.TASK)
             {
                 //~ Entity is a player so use that name instead
                 if (editableEntity.GetPlayerID() > 0)
                     return GetPlayerName(editableEntity.GetPlayerID(), entityName);

                 if (useCharacterName)
                 {
                     if (SCR_EditableCharacterComponent.Cast(editableEntity))
                     {
                         string format, firstname, alias, surname;
                         if (GetCharacterName(entityRplID, format, firstname, alias, surname))
                         {
                             entityName = WidgetManager.Translate(format, firstname, alias, surname);
                             if (!entityName.IsEmpty())
                                 return true;
                         }
                     }
                 }

                 //~ Use entity name
                 entityName = editableEntity.GetDisplayName();
             }
             //~ Get objective type name
             else
             {
                 SCR_EditableTaskComponentClass taskPrefabData = SCR_EditableTaskComponentClass.Cast(editableEntity.GetComponentData(editableEntity.GetOwner()));
                 if (taskPrefabData)
                     entityName = taskPrefabData.GetObjectiveTypeName();
                 else
                     entityName = editableEntity.GetDisplayName();
             }
         }

         return !entityName.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
     //~ Returns faction name by index
     protected bool GetFactionName(int factionIndex, out string factionName)
     {
         //~ Name already assigned
         if (!factionName.IsEmpty())
             return true;

         FactionManager factionManager = GetGame().GetFactionManager();
         if (!factionManager)
             return false;

         Faction faction = factionManager.GetFactionByIndex(factionIndex);
         if (!faction)
             return false;

         factionName = faction.GetFactionName();
         return !factionName.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
     protected bool GetCharacterName(int entityRplID, out string format, out string firstname, out string alias, out string surname)
     {
         //Get target Entity. Also works with non-editable entity iD
         SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(Replication.FindItem(entityRplID));
         if (!editableEntity)
         {
             IEntity entity = IEntity.Cast(Replication.FindItem(entityRplID));
             if (!entity)
             {
                 RplComponent rplComponent = RplComponent.Cast(Replication.FindItem(entityRplID));
                 if (rplComponent)
                     entity = rplComponent.GetEntity();
             }
             if (entity)
                 editableEntity = SCR_EditableEntityComponent.Cast(entity.FindComponent(SCR_EditableEntityComponent));

             if (!editableEntity)
                 return false;
         }

         SCR_CharacterIdentityComponent charIdentity = SCR_CharacterIdentityComponent.Cast(editableEntity.GetOwner().FindComponent(SCR_CharacterIdentityComponent));
         if (!charIdentity)
             return false;

         charIdentity.GetFormattedFullName(format, firstname, alias, surname);
         return true;
     }

     //------------------------------------------------------------------------------------------------
     //~ Returns translated callsign in correct formatting
     protected bool GetEntityCallsign(int rplID, out string callsign)
     {
         //~ Name already assigned
         if (!callsign.IsEmpty())
             return true;

         Managed managed = Replication.FindItem(rplID);
         if (!managed)
             return false;

         SCR_CallsignBaseComponent callsignComponent;
         ScriptComponent scriptComp = ScriptComponent.Cast(managed);

         //~ Get managed script
         if (scriptComp)
         {
             callsignComponent = SCR_CallsignBaseComponent.Cast(scriptComp.GetOwner().FindComponent(SCR_CallsignBaseComponent));
         }
         //~ Get managed entity
         else
         {
             IEntity entity = IEntity.Cast(managed);
             if (!entity)
                 return false;

             callsignComponent =  SCR_CallsignBaseComponent.Cast(entity.FindComponent(SCR_CallsignBaseComponent));
         }

         if (!callsignComponent)
             return false;

         string company, platoon, squad, character, format;
         if (!callsignComponent.GetCallsignNames(company, platoon, squad, character, format))
             return false;

         //~ Translate the callsign strings so it can be send as 1 string
         callsign = WidgetManager.Translate(format, company, platoon, squad, character);
         return true;
     }

     //------------------------------------------------------------------------------------------------
     //~ Returns group name, or callsign if no name assigned
     protected bool GetGroupNameFromGroupID(int playerGroupId, out string groupName)
     {
         //~ Name already assigned
         if (!groupName.IsEmpty())
             return true;

         SCR_GroupsManagerComponent groupsManager = SCR_GroupsManagerComponent.GetInstance();
         if (!groupsManager)
             return false;

         SCR_AIGroup aiGroup;
         aiGroup = groupsManager.FindGroup(playerGroupId);

         if (!aiGroup)
             return false;

         groupName = aiGroup.GetCustomName();

         //~ No custom name set so get Callsign instead
         if (SCR_StringHelper.IsEmptyOrWhiteSpace(groupName))
             return GetEntityCallsign(Replication.FindId(aiGroup), groupName);

         return true;
     }

     //------------------------------------------------------------------------------------------------
     //~ Get inventory item name
     protected bool GetInventoryItemName(RplId rplId, out string itemName)
     {
         if (!itemName.IsEmpty())
             return true;

         RplComponent itemRplcComp = RplComponent.Cast(Replication.FindItem(rplId));
         if (!itemRplcComp)
             return false;

         IEntity itemEntity = itemRplcComp.GetEntity();
         if (!itemEntity)
             return false;

         InventoryItemComponent item = InventoryItemComponent.Cast(itemEntity.FindComponent(InventoryItemComponent));
         if (!item)
             return false;

         ItemAttributeCollection attributeCollection = item.GetAttributes();
         if (!ItemAttributeCollection)
             return false;

         UIInfo uiInfo = attributeCollection.GetUIInfo();
         if (!uiInfo)
             return false;

         itemName = uiInfo.GetName();

         return !itemName.IsEmpty();
     }

     //------------------------------------------------------------------------------------------------
  bool GetPosition(SCR_NotificationData data, out vector position)
     {
         if (m_info.GetEditorSetPositionData() == ENotificationSetPositionData.AUTO_SET_AND_UPDATE_POSITION)
             SetPosition(data);

         data.GetPosition(position);

         if (position == vector.Zero)
             return false;

         return true;
     }

     //------------------------------------------------------------------------------------------------
  void SetPosition(SCR_NotificationData data)
     {

     }

     //------------------------------------------------------------------------------------------------
     protected bool CanSetPosition(SCR_NotificationData data)
     {
         vector position;
         data.GetPosition(position);

         return !((m_info.GetEditorSetPositionData() == ENotificationSetPositionData.AUTO_SET_POSITION_ONCE && position != vector.Zero) || m_info.GetEditorSetPositionData() == ENotificationSetPositionData.NEVER_AUTO_SET_POSITION);
     }

     //------------------------------------------------------------------------------------------------
     //Set position data using Player ID
     protected void SetPositionDataEditablePlayer(int playerID, SCR_NotificationData data)
     {
         PlayerManager playerManager = GetGame().GetPlayerManager();
         if (!playerManager)
             return;

         SCR_EditableEntityComponent playerEntity = SCR_EditableEntityComponent.GetEditableEntity(playerManager.GetPlayerControlledEntity(playerID));

         if (!playerEntity)
             return;

         vector position;

         if (!playerEntity.GetPos(position))
             return;

         data.SetPosition(position);
     }

     //------------------------------------------------------------------------------------------------
     //Set position data using Editable Entity ID
     protected void SetPositionDataEditableEntity(int enditableEntityID, SCR_NotificationData data)
     {
         SCR_EditableEntityComponent targetEntity = SCR_EditableEntityComponent.Cast(Replication.FindItem(enditableEntityID));

         if (!targetEntity) return;

         vector position;
         if (!targetEntity.GetPos(position)) return;

         data.SetPosition(position);
     }

     //------------------------------------------------------------------------------------------------
  void SetFactionRelatedColor(SCR_NotificationData data)
     {

     }

     //------------------------------------------------------------------------------------------------
     //Get faction related color of player target
     protected ENotificationColor GetFactionRelatedColorPlayer(int notificationPlayerID, ENotificationColor colorType)
     {
         //If not a faction set color keep current color
         if (colorType < ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
             return colorType;

         if (!GetGame().GetPlayerController())
             return ENotificationColor.NEUTRAL;

         SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
         if (!factionManager)
             return ENotificationColor.NEUTRAL;

         //Get factions using ID
         Faction playerSelfFaction = factionManager.GetLocalPlayerFaction();
         Faction notificationPlayerFaction = factionManager.GetPlayerFaction(notificationPlayerID);

         if (!playerSelfFaction || !notificationPlayerFaction)
             return ENotificationColor.NEUTRAL;

         //Check if friendly
         if (playerSelfFaction.IsFactionFriendly(notificationPlayerFaction))
         {
             if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
                 return ENotificationColor.NEGATIVE;
             else if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_POSITIVE)
                 return ENotificationColor.POSITIVE;
             else if (colorType == ENotificationColor.FACTION_ENEMY_IS_NEGATIVE_ONLY)
                 return ENotificationColor.NEUTRAL;
             else if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_POSITIVE_ONLY)
                 return ENotificationColor.POSITIVE;
         }
         else
         {
             if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
                 return ENotificationColor.POSITIVE;
             else if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_POSITIVE)
                 return ENotificationColor.NEGATIVE;
             else if (colorType == ENotificationColor.FACTION_ENEMY_IS_NEGATIVE_ONLY)
                 return ENotificationColor.NEGATIVE;
             else if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_POSITIVE_ONLY)
                 return ENotificationColor.NEUTRAL;
         }

         //No color set
         return ENotificationColor.NEUTRAL;
     }

     //------------------------------------------------------------------------------------------------
     //Get faction related color of entity target
     protected ENotificationColor GetFactionRelatedColorEntity(int notificationEntityID, ENotificationColor colorType)
     {
         //If not a faction set color keep current color
         if (colorType < ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
             return colorType;

         if (!GetGame().GetPlayerController()) return ENotificationColor.NEUTRAL;
         SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
         if (!factionManager) return ENotificationColor.NEUTRAL;

         //Get notification entity
         SCR_EditableEntityComponent notificationEntity = SCR_EditableEntityComponent.Cast(Replication.FindItem(notificationEntityID));
         if (!notificationEntity) return ENotificationColor.NEUTRAL;

         //Get notification entity ChimeraCharacter
         SCR_ChimeraCharacter notificationEntityChimera = SCR_ChimeraCharacter.Cast(notificationEntity.GetOwner());
         if (!notificationEntityChimera) return ENotificationColor.NEUTRAL;

         //Get factions
         Faction playerSelfFaction = factionManager.GetLocalPlayerFaction();
         Faction notificationEntityFaction = notificationEntityChimera.GetFaction();

         if (!playerSelfFaction || !notificationEntityFaction) return ENotificationColor.NEUTRAL;

         //Check if friendly
         if (playerSelfFaction.IsFactionFriendly(notificationEntityFaction))
         {
             if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
                 return ENotificationColor.NEGATIVE;
             else if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_POSITIVE)
                 return ENotificationColor.POSITIVE;
             else if (colorType == ENotificationColor.FACTION_ENEMY_IS_NEGATIVE_ONLY)
                 return ENotificationColor.NEUTRAL;
             else if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_POSITIVE_ONLY)
                 return ENotificationColor.POSITIVE;
         }
         else
         {
             if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_NEGATIVE)
                 return ENotificationColor.POSITIVE;
             else if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_POSITIVE)
                 return ENotificationColor.NEGATIVE;
             else if (colorType == ENotificationColor.FACTION_ENEMY_IS_NEGATIVE_ONLY)
                 return ENotificationColor.NEGATIVE;
             else if (colorType == ENotificationColor.FACTION_FRIENDLY_IS_POSITIVE_ONLY)
                 return ENotificationColor.NEUTRAL;
         }

         //No color set
         return ENotificationColor.NEUTRAL;
     }

     //------------------------------------------------------------------------------------------------
     //Check if entities are friendly
     protected bool AreEntitiesFriendly(int entity1ID, bool entity1IsPlayer, int entity2ID, bool entity2IsPlayer)
     {
         Faction faction1, faction2;

         SCR_FactionManager factionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());

         if (entity1IsPlayer || entity2IsPlayer)
         {
             if (!factionManager)
                 return false;
         }

         SCR_EditableEntityComponent entity;
         SCR_ChimeraCharacter entityChimera;

         //Get notification entity

         //Get entity 1 faction
         if (entity1IsPlayer)
         {
             faction1 = factionManager.GetPlayerFaction(entity1ID);
         }
         else
         {
             entity = SCR_EditableEntityComponent.Cast(Replication.FindItem(entity1ID));
             if (!entity)
                 return false;

             //Get notification entity SCR_ChimeraCharacter
             entityChimera = SCR_ChimeraCharacter.Cast(entity.GetOwner());
             if (!entityChimera)
                 return false;

             faction1 = entityChimera.GetFaction();
         }

         //Get entity 2 faction
         if (entity2IsPlayer)
         {
             faction2 = factionManager.GetPlayerFaction(entity2ID);
         }
         else
         {
             entity = SCR_EditableEntityComponent.Cast(Replication.FindItem(entity2ID));
             if (!entity)
                 return false;

             //Get notification entity SCR_ChimeraCharacter
             entityChimera = SCR_ChimeraCharacter.Cast(entity.GetOwner());
             if (!entityChimera)
                 return false;

             faction2 = entityChimera.GetFaction();
         }

         if (!faction1 || !faction2)
             return false;

         return faction1.IsFactionFriendly(faction2);
     }
 };