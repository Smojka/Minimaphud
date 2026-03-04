 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class TagSystem: GameSystem
 {
     proto external int GetTagsInRange(out notnull array<IEntity> entities, vector origin, float range, ETagCategory categories);
     proto external void RegisterEntity(IEntity entity, bool isDynamic, ETagCategory categories);
     proto external void UnregisterEntity(IEntity entity);
 }
