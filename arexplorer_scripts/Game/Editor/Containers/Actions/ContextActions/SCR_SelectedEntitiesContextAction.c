 //------------------------------------------------------------------------------------------------
 [BaseContainerProps(), SCR_BaseContainerCustomTitleUIInfo("m_Info")]
 class SCR_SelectedEntitiesContextAction : SCR_BaseContextAction
 {
     override bool CanBeShown(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
     {
         foreach (SCR_EditableEntityComponent entity : selectedEntities)
         {
             if (CanBeShown(entity, cursorWorldPosition, flags))
             {
                 return true;
             }
         }
         return false;
     }

     override bool CanBePerformed(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags)
     {
         foreach (SCR_EditableEntityComponent entity : selectedEntities)
         {
             if (CanBePerformed(entity, cursorWorldPosition, flags))
             {
                 return true;
             }
         }
         return false;
     }

     override void Perform(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
     {
         if (!InitPerform()) return;

         foreach (SCR_EditableEntityComponent entity : selectedEntities)
         {
             if (CanBePerformed(entity, cursorWorldPosition, 0))
             {
                 Perform(entity, cursorWorldPosition);
             }
         }
     }

     override void PerformOwner(SCR_EditableEntityComponent hoveredEntity, notnull set<SCR_EditableEntityComponent> selectedEntities, vector cursorWorldPosition, int flags, int param = -1)
     {
         if (!InitPerform()) return;

         foreach (SCR_EditableEntityComponent entity : selectedEntities)
         {
             if (CanBePerformed(entity, cursorWorldPosition, 0))
             {
                 PerformOwner(entity, cursorWorldPosition);
             }
         }
     }

     bool CanBeShown(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
     {
         return true;
     }

     bool CanBePerformed(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition, int flags)
     {
         return true;
     }

     bool InitPerform()
     {
         return true;
     }

     void Perform(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
     {

     }

     void PerformOwner(SCR_EditableEntityComponent selectedEntity, vector cursorWorldPosition)
     {

     }
 };