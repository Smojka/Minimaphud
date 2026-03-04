 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class RadioManagerEntityClass: GenericEntityClass
 {
 }

 class RadioManagerEntity: GenericEntity
 {
     proto external int GetTransceiversInRange(vector position, float range, out notnull array<BaseTransceiver> tsvList);
 }
