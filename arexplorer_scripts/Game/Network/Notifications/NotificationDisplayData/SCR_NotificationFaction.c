
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationFaction : SCR_NotificationDisplayData
 {
     override string GetText(SCR_NotificationData data)
     {
         int factionEntityID;
         data.GetParams(factionEntityID);

         SCR_EditableEntityComponent factionEntity = SCR_EditableEntityComponent.Cast(Replication.FindItem(factionEntityID));
         if (!factionEntity) return string.Empty;

         Faction faction = factionEntity.GetFaction();
         if (!faction) return string.Empty;

         data.SetNotificationTextEntries(faction.GetUIInfo().GetName());
         return super.GetText(data);
     }
 };