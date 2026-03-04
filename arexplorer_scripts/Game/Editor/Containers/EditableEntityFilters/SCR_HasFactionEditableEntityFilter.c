 [BaseContainerProps(), SCR_BaseContainerCustomTitleEnum(EEditableEntityState, "m_State")]
 class SCR_HasFactionEditableEntityFilter : SCR_BaseEditableEntityFilter
 {
     override bool CanAdd(SCR_EditableEntityComponent entity)
     {
         return (entity.GetFaction() || entity.GetAIGroup()) && !entity.GetPlayerID(); //--- Check for presence of a group as well, faction may not be defined yet upon creation
     }
 };