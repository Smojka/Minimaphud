
 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(ENotification, "m_NotificationKey")]
 class SCR_NotificationEntityCallsign : SCR_NotificationEditableEntity
 {
     override string GetText(SCR_NotificationData data)
     {
         int entityID;
         data.GetParams(entityID);

         string callsign;
         data.GetNotificationTextEntries(callsign);
         if (!GetEntityCallsign(entityID, callsign))
             return string.Empty;

         data.SetNotificationTextEntries(callsign);
         return super.GetText(data);
     }
 };