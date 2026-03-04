 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class MineManagerComponentClass: GenericComponentClass
 {
 }

 class MineManagerComponent: GenericComponent
 {
     proto external void Register(notnull IEntity mine, IEntity mineSpawner);
     proto external void Remove(notnull IEntity mine);
     proto external void AddFactionAwareness(notnull IEntity mine, notnull IEntity awarenessAdder);
 }
