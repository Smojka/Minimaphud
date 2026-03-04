 [BaseContainerProps()]
 class SCR_BasePlacingToolbarAction : SCR_EditorToolbarAction
 {
     [Attribute("", UIWidgets.ResourceNamePicker, "Prefab", "et")]
     ResourceName m_PlaceablePrefab;

     override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
     {
         Print("CanBeShown condition method not overridden for action, action won't show", LogLevel.WARNING);
         return false;
     }

     override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
     {
         return true;
     }

     override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition,int flags, int param = -1)
     {

     }

     void PerformCancel()
     {

     }

     void PerformFinish(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, SCR_EditableEntityComponent placedEntity)
     {

     }

     bool GetPrefab(out ResourceName prefab)
     {
         prefab = m_PlaceablePrefab;
         return !m_PlaceablePrefab.IsEmpty();
     }
 };