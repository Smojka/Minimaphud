
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationInventoryItemName : SCR_NotificationDisplayData
 {
     override string GetText(SCR_NotificationData data)
     {
         RplId itemReplicationId;
         data.GetParams(itemReplicationId);

         string inventoryItemName;
         data.GetNotificationTextEntries(inventoryItemName);

         if (!GetInventoryItemName(itemReplicationId, inventoryItemName))
             return string.Empty;

         data.SetNotificationTextEntries(inventoryItemName);
         return super.GetText(data);
     }
 };