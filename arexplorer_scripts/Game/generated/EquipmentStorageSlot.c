 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class EquipmentStorageSlot: InventoryStorageSlot
 {
     proto external bool CanAttachItem(IEntity item);
     proto external bool IsOccluded();

     // callbacks

     event protected bool OnOccludedStateChanged(bool occluded) { return false; };
 }