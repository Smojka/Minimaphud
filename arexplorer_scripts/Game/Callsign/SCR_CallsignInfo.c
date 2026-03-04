
 [BaseContainerProps(), SCR_BaseContainerCustomTitleField("m_sCallsign")]
 class SCR_CallsignInfo
 {
     [Attribute()]
     protected LocalizedString m_sCallsign;

     string GetCallsign()
     {
         return m_sCallsign;
     }
 };