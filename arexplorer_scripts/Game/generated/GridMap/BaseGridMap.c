 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class BaseGridMap: Managed
 {
     void BaseGridMap(float cellSize = 100.0) {}

     proto external void UpdatePosition(notnull IEntity entity, vector newPosition);
     proto external void Remove(IEntity entity);
     proto external void Update();
     proto external int FindEntitiesInRange(out notnull array<IEntity> entities, vector origin, float range, int mask = 0);
     proto external void SetUpdateOfStaticEntities(bool state);
     proto external void SetNmbOfStaticEntitiesUpdated(int nmb);
 }
