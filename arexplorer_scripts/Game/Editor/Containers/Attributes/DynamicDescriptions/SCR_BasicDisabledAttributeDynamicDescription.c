 [BaseContainerProps(), BaseContainerCustomStringTitleField("Basic Disabled")]
 class SCR_BasicDisabledAttributeDynamicDescription : SCR_BaseAttributeDynamicDescription
 {
     //------------------------------------------------------------------------------------------------
     override bool IsValid(notnull SCR_BaseEditorAttribute attribute, notnull SCR_BaseEditorAttributeUIComponent attributeUi)
     {
         return !attribute.IsEnabled();
     }
 };
