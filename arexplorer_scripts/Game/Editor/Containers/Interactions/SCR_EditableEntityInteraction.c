 [BaseContainerProps()]
 class SCR_EditableEntityInteraction
 {
     static const int ROOT = -1;

     sealed bool CanSetParent(SCR_EditableEntityComponent parentEntity, EEditableEntityInteractionFlag interactionFlags = int.MAX)
     {
         if (parentEntity)
             return CanSetParent(parentEntity.GetEntityType(), parentEntity.GetEntityFlags(), interactionFlags);
         else
             return CanSetParent(ROOT, 0, interactionFlags);
     }
     bool CanSetParent(EEditableEntityType parentType, EEditableEntityFlag parentFlags, EEditableEntityInteractionFlag interactionFlags = int.MAX)
     {
         if (!SCR_Enum.HasFlag(interactionFlags, EEditableEntityInteractionFlag.DELEGATE) || !SCR_Enum.HasFlag(interactionFlags, EEditableEntityInteractionFlag.LAYER_EDITING))
             return false;

         if (parentType == ROOT)
             return true;

         if (!SCR_Enum.HasFlag(interactionFlags, EEditableEntityInteractionFlag.DELEGATE))
             return false;

         if ((parentFlags & EEditableEntityFlag.LAYER) != EEditableEntityFlag.LAYER)
             return false;

         return parentType == EEditableEntityType.GENERIC;
     }
     bool CanCreateParentFor(EEditableEntityType newLayerType, EEditableEntityFlag newLayerFlags, SCR_EditableEntityComponent parentEntity, bool CheckParentEntity = true)
     {
         if (CheckParentEntity && parentEntity)
         {
             SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));

             if (core)
             {
                 SCR_EditableEntityInteraction interaction = core.GetEntityInteraction(EEditableEntityType.GENERIC);
                 if (!interaction.CanSetParent(parentEntity))
                     return false;
             }
         }

         return CanSetParent(newLayerType, newLayerFlags);
     }
 };