 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class SlotManagerComponentClass: GameComponentClass
 {
 }

 class SlotManagerComponent: GameComponent
 {
     proto external int GetSlotInfos(out notnull array<EntitySlotInfo> outSlotInfos);
     proto external EntitySlotInfo GetSlotByName(string slotName);
 }
