 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class AABGridMap: BaseGridMap
 {
     void AABGridMap(float cellSize = 100.0) {}

     proto external void Insert(IEntity entity, vector mins, vector maxs, bool isDynamic, int mask = 0);
     proto external void UpdateAABB(IEntity entity, vector mins, vector maxs);
 }
