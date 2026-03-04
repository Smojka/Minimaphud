 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class InventoryStorageSlot: EntitySlotInfo
 {
     proto external bool ScriptOnRplSave(ScriptBitWriter writer);
     proto external bool ScriptOnRplLoad(ScriptBitReader reader);
     proto external vector GetInspectionWidgetOffset();
     proto external bool IsLocked();
     proto external int GetID();
     proto external GenericComponent GetParentContainer();
     proto external BaseInventoryStorageComponent GetStorage();
 }
