
 class SCR_NotificationData
 {
     //--- Local params
     protected ENotification m_iID;
     protected float m_fNotificationTimeLeft = -1;
     protected int m_iPrevPositionUpdated = -1;
     protected string notificationEntry1, notificationEntry2, notificationEntry3, notificationEntry4, notificationEntry5, notificationEntry6;
     protected bool m_bIsSplitNotification;
     protected ENotificationColor m_FactionRelatedColor = ENotificationColor.NEUTRAL;
     protected ENotificationColor m_FactionRelatedTextColor = ENotificationColor.NEUTRAL;
     protected ENotificationColor m_FactionRelatedTextLeftColor = ENotificationColor.NEUTRAL;
     protected ENotificationColor m_FactionRelatedTextRightColor = ENotificationColor.NEUTRAL;
     protected SCR_NotificationDisplayData m_DisplayData;

     //--- Replicated params
     protected int m_iParam1;
     protected int m_iParam2;
     protected int m_iParam3;
     protected int m_iParam4;
     protected int m_iParam5;
     protected int m_iParam6;
     protected vector m_vPosition = vector.Zero;
     protected ENotificationReceiver m_iNotificationReceiverType;

     void SetMeta(int id, SCR_NotificationDisplayData displayData)
     {
         m_iID = id;

         //Set how much time left before notification will be deleted from data
         m_fNotificationTimeLeft = SCR_NotificationsComponent.NOTIFICATION_DELETE_TIME;

         m_DisplayData = displayData;

         if (m_DisplayData)
         {
             if (!m_DisplayData.m_info)
             {
                 Print("(" + typename.EnumToString(ENotification, GetID()) + ") SCR_NotificationDisplayData has no UIInfo assigned! This will cause errors!", LogLevel.ERROR);
                 m_DisplayData.m_info = new SCR_UINotificationInfo;
             }

             //Set any initial local data such as initial notification position and faction related notification color
             m_DisplayData.SetInitialDisplayData(this);
         }
     }

     int GetID()
     {
         return m_iID;
     }

     ENotificationReceiver GetNotificationReceiverType()
     {
         return m_iNotificationReceiverType;
     }

     float GetNotificationTimeLeft()
     {
         return m_fNotificationTimeLeft;
     }

     bool UpdateNotificationData(float timeSlice)
     {
         m_fNotificationTimeLeft -= timeSlice;

         //Update Position ever second the notification data exists
         if (m_DisplayData && m_DisplayData.GetNotificationUIInfo().GetEditorSetPositionData() == ENotificationSetPositionData.AUTO_SET_AND_UPDATE_POSITION)
         {
             if (m_iPrevPositionUpdated != (int)m_fNotificationTimeLeft)
             {
                 m_iPrevPositionUpdated = m_fNotificationTimeLeft;
                 m_DisplayData.SetPosition(this);
             }
         }

         //No time left so delete
         return (m_fNotificationTimeLeft <= 0);
     }

     string GetText()
     {
         if (m_DisplayData)
         {
             return m_DisplayData.GetText(this);
         }
         else
         {
             Print("(" + typename.EnumToString(ENotification, GetID()) + ") SCR_NotificationDisplayData has no m_DisplayData assigned!", LogLevel.ERROR);
             return typename.EnumToString(ENotification, GetID());
         }
     }

     void SetNotificationTextEntries(string entry1 = string.Empty, string entry2 = string.Empty, string entry3 = string.Empty, string entry4 = string.Empty, string entry5 = string.Empty, string entry6 = string.Empty)
     {
         if (entry1 != string.Empty)
             notificationEntry1 = entry1;
         if (entry2 != string.Empty)
             notificationEntry2 = entry2;
         if (entry3 != string.Empty)
             notificationEntry3 = entry3;
         if (entry4 != string.Empty)
             notificationEntry4 = entry4;
         if (entry5 != string.Empty)
             notificationEntry5 = entry5;
         if (entry6 != string.Empty)
             notificationEntry6 = entry6;
     }

     void GetNotificationTextEntries(out string entry1 = string.Empty, out string entry2 = string.Empty, out string entry3 = string.Empty, out string entry4 = string.Empty, out string entry5 = string.Empty, out string entry6 = string.Empty)
     {
         entry1 = notificationEntry1;
         entry2 = notificationEntry2;
         entry3 = notificationEntry3;
         entry4 = notificationEntry4;
         entry5 = notificationEntry5;
         entry6 = notificationEntry6;
     }

     void SetFactionRelatedColor(ENotificationColor factionRelatedColor)
     {
         m_FactionRelatedColor = factionRelatedColor;
     }

     void SetFactionRelatedTextColor(ENotificationColor factionRelatedColor)
     {
         m_FactionRelatedTextColor = factionRelatedColor;
     }

     void SetSplitFactionRelatedColor(ENotificationColor factionRelatedColorLeft, ENotificationColor factionRelatedColorRight)
     {
         m_FactionRelatedTextLeftColor = factionRelatedColorLeft;
         m_FactionRelatedTextRightColor = factionRelatedColorRight;
     }

     ENotificationColor GetFactionRelatedColor()
     {
         return m_FactionRelatedColor;
     }

     ENotificationColor GetFactionRelatedTextColor()
     {
         return m_FactionRelatedTextColor;
     }


     void GetSplitFactionRelatedColor(out ENotificationColor leftTextColor, out ENotificationColor rightTextColor)
     {
         leftTextColor = m_FactionRelatedTextLeftColor;
         rightTextColor = m_FactionRelatedTextRightColor;
     }

     SCR_NotificationDisplayData GetDisplayData()
     {
         return m_DisplayData;
     }

     void GetPosition(out vector position)
     {
         position = m_vPosition;
     }

     void GetParams(out int param1, out int param2 = 0, out int param3 = 0, out int param4 = 0, out int param5 = 0, out int param6 = 0)
     {
         param1 = m_iParam1;
         param2 = m_iParam2;
         param3 = m_iParam3;
         param4 = m_iParam4;
         param5 = m_iParam5;
         param6 = m_iParam6;
     }

     void Log()
     {
         PrintFormat("%1\nID: %2\nParams: %3, %4, %5, %6, %7, %8", this, typename.EnumToString(ENotification, m_iID), m_iParam1, m_iParam2, m_iParam3, m_iParam4, m_iParam5, m_iParam6);
     }

     static void Encode(SSnapSerializerBase snapshot, ScriptCtx hint, ScriptBitSerializer packet)
     {
         snapshot.Serialize(packet, 40);
     }
     static bool Decode(ScriptBitSerializer packet, ScriptCtx hint, SSnapSerializerBase snapshot)
     {
         return snapshot.Serialize(packet, 40);
     }
     static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx hint)
     {
         return lhs.CompareSnapshots(rhs, 40);
     }
     static bool PropCompare(SCR_NotificationData prop, SSnapSerializerBase snapshot, ScriptCtx hint)
     {
         return snapshot.Compare(prop.m_iParam1, 4)
             && snapshot.Compare(prop.m_iParam2, 4)
             && snapshot.Compare(prop.m_iParam3, 4)
             && snapshot.Compare(prop.m_iParam4, 4)
             && snapshot.Compare(prop.m_iParam5, 4)
             && snapshot.Compare(prop.m_iParam6, 4)
             && snapshot.Compare(prop.m_vPosition, 12)
             && snapshot.Compare(prop.m_iNotificationReceiverType, 4);
     }
     static bool Extract(SCR_NotificationData prop, ScriptCtx hint, SSnapSerializerBase snapshot)
     {
         snapshot.SerializeBytes(prop.m_iParam1, 4);
         snapshot.SerializeBytes(prop.m_iParam2, 4);
         snapshot.SerializeBytes(prop.m_iParam3, 4);
         snapshot.SerializeBytes(prop.m_iParam4, 4);
         snapshot.SerializeBytes(prop.m_iParam5, 4);
         snapshot.SerializeBytes(prop.m_iParam6, 4);
         snapshot.SerializeBytes(prop.m_vPosition, 12);
         snapshot.SerializeBytes(prop.m_iNotificationReceiverType, 4);
         return true;
     }
     static bool Inject(SSnapSerializerBase snapshot, ScriptCtx hint, SCR_NotificationData prop)
     {
         snapshot.SerializeBytes(prop.m_iParam1, 4);
         snapshot.SerializeBytes(prop.m_iParam2, 4);
         snapshot.SerializeBytes(prop.m_iParam3, 4);
         snapshot.SerializeBytes(prop.m_iParam4, 4);
         snapshot.SerializeBytes(prop.m_iParam5, 4);
         snapshot.SerializeBytes(prop.m_iParam6, 4);
         snapshot.SerializeBytes(prop.m_vPosition, 12);
         snapshot.SerializeBytes(prop.m_iNotificationReceiverType, 4);
         return true;
     }

     void SetPosition(vector position)
     {
         m_vPosition = position;
     }

     void SetParameters(ENotificationReceiver receiverType, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, int param5 = 0, int param6 = 0)
     {
         m_iNotificationReceiverType = receiverType;
         m_iParam1 = param1;
         m_iParam2 = param2;
         m_iParam3 = param3;
         m_iParam4 = param4;
         m_iParam5 = param5;
         m_iParam6 = param6;
     }

 };