 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class EquipedLoadoutStorageComponentClass: ScriptedBaseInventoryStorageComponentClass
 {
 }

 class EquipedLoadoutStorageComponent: ScriptedBaseInventoryStorageComponent
 {
     proto external LoadoutSlotInfo GetSlotFromArea(typename pAreaType);
     proto external int GetSlotsFromArea(typename pAreaType, out array<ref LoadoutSlotInfo> outSlots);
     proto external IEntity GetClothFromArea(typename pAreaType);
 }
