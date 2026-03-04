
 [BaseContainerProps(), BaseContainerCustomStringTitleField("DO NOT USE BASE CLASS")]
 class SCR_BaseButtonAttributeDynamicDescription : SCR_BaseAttributeDynamicDescription
 {
     [Attribute("1", desc: "If true it then it has priority over button descriptions if hovering over a attribute button, making sure it is always shown first")]
     protected bool m_bPriorityOverButtonDescription;

     //------------------------------------------------------------------------------------------------
  bool HasPriorityOverButton()
     {
         return m_bPriorityOverButtonDescription;
     }
 };