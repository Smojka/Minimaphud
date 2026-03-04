 class SCR_EquipmentStorageComponentClass: EquipmentStorageComponentClass
 {
 }

 class SCR_EquipmentStorageComponent : EquipmentStorageComponent
 {
     //------------------------------------------------------------------------------------------------
  override protected bool ShouldPreviewAttachedItems()
     {
         return false;
     }
 }

 class SCR_EquipmentStorageSlot : EquipmentStorageSlot
 {
     //------------------------------------------------------------------------------------------------
     override protected bool OnOccludedStateChanged(bool occluded)
     {
         IEntity ent = GetAttachedEntity();
         if (ent)
         {
             SCR_GadgetComponent gadgetComp = SCR_GadgetComponent.Cast( ent.FindComponent(SCR_GadgetComponent) );
             if (gadgetComp && gadgetComp.IsVisibleEquipped())
                 return false;   // let the slot handle occlusion if the gadget can be visible
             else
                 return true;    // if it is slot which is not visualized, dont do any visibility changes (keep invis)
         }

         return false;
     }
 }