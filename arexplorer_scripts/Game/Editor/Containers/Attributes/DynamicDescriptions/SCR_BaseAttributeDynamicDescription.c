
 [BaseContainerProps(), BaseContainerCustomStringTitleField("DO NOT USE BASE CLASS")]
 class SCR_BaseAttributeDynamicDescription
 {
     [Attribute(desc: "It uses color, icon and description to display dynamic description")]
     protected ref SCR_EditorAttributeUIInfo m_DescriptionDisplayInfo;

     [Attribute("1", desc: "If false than it will never be shown")]
     protected bool m_bEnabled;

     //------------------------------------------------------------------------------------------------
  void InitDynamicDescription(notnull SCR_BaseEditorAttribute attribute, notnull SCR_BaseEditorAttributeUIComponent attributeUi);

     //------------------------------------------------------------------------------------------------
  bool IsValid(notnull SCR_BaseEditorAttribute attribute, notnull SCR_BaseEditorAttributeUIComponent attributeUi)
     {
         return m_bEnabled;
     }

     //------------------------------------------------------------------------------------------------
  void GetDescriptionData(notnull SCR_BaseEditorAttribute attribute, notnull SCR_BaseEditorAttributeUIComponent attributeUi, out SCR_EditorAttributeUIInfo uiInfo, out string param1 = string.Empty, out string param2 = string.Empty, out string param3 = string.Empty)
     {
         uiInfo = m_DescriptionDisplayInfo;
     }
 };
